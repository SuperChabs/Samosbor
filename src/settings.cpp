#include "settings.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json.hpp>

void Settings::LoadDefaults()
{
    isRunning = true;
    encodedCoins = EncodeCoins(0);
    currentLevel = 1;
    hasActiveSave = false;
    health = 100;       
    maxHealth = 100;     
}

// Constructor: ensure sensible defaults immediately when instance is created.
Settings::Settings()
{
    LoadDefaults();
}

void Settings::ResetProgress()
{
    encodedCoins = EncodeCoins(0);
    currentLevel = 1;
    hasActiveSave = false;
    health = 100;        
    maxHealth = 100;    
}

void Settings::SaveProgress()
{
    hasActiveSave = true;
    SaveToFile(DefaultConfigPath());
}

using json = nlohmann::json;

bool Settings::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        // If file doesn't exist, create default config and save it to the chosen path.
        LoadDefaults();
        SaveToFile(filename.empty() ? std::string("config.json") : filename);
        return true;
    }

    try {
        json j;
        file >> j;
        file.close();

        // Always start running when loading; we don't persist the 'running' flag across restarts.
        isRunning = true;

        if (j.contains("coins") && j["coins"].is_number_integer()) {
            SetCoins(j["coins"].get<int>());
        }
        if (j.contains("level") && j["level"].is_number_integer()) {
            currentLevel = j["level"].get<int>();
        }
        if (j.contains("has_save")) {
            if (j["has_save"].is_boolean()) hasActiveSave = j["has_save"].get<bool>();
            else if (j["has_save"].is_number_integer()) hasActiveSave = (j["has_save"].get<int>() != 0);
        }
        if (j.contains("health") && j["health"].is_number_integer()) {
            health = j["health"].get<int>();
        }
        if (j.contains("max_health") && j["max_health"].is_number_integer()) {
            maxHealth = j["max_health"].get<int>();
        }

        return true;
    } catch (const std::exception& e) {
        // On parse error, keep defaults and return false
        file.close();
        return false;
    }
}

bool Settings::SaveToFile(const std::string& filename)
{
    std::string outname = filename.empty() ? std::string("config.json") : filename;

    // Ensure parent directory exists if using a nested path
    try {
        std::filesystem::path p(outname);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }
    } catch (...) {
        // ignore filesystem errors; we'll try to open file and report failure
    }

    std::ofstream file(outname);
    if (!file.is_open()) return false;

    // Use nlohmann::json to build object and write prettified output.
    try {
        json j;
        // Do not persist 'running' to avoid preventing restart; store human-readable coin count.
        j["coins"] = GetCoins();
        j["level"] = currentLevel;
        j["health"] = health;
        j["max_health"] = maxHealth;
        j["has_save"] = hasActiveSave;

        // For safer write: write to temp file then rename
        std::filesystem::path p(outname);
        std::filesystem::path tmp = p;
        tmp += ".tmp";

        std::ofstream tmpf(tmp, std::ios::trunc);
        if (!tmpf.is_open()) return false;
        tmpf << j.dump(2) << "\n";
        tmpf.close();

        std::error_code ec;
        std::filesystem::rename(tmp, p, ec);
        if (ec) {
            // fallback: try to copy
            std::filesystem::copy_file(tmp, p, std::filesystem::copy_options::overwrite_existing, ec);
            std::filesystem::remove(tmp);
            if (ec) return false;
        }

        return true;
    } catch (...) {
        return false;
    }
}

std::string Settings::DefaultConfigPath()
{
    const char* xdg = std::getenv("XDG_CONFIG_HOME");
    std::filesystem::path p;
    if (xdg && xdg[0] != '\0') {
        p = std::filesystem::path(xdg) / "Samosbor" / "config.json";
    } else {
        const char* home = std::getenv("HOME");
        if (home && home[0] != '\0') {
            p = std::filesystem::path(home) / ".config" / "Samosbor" / "config.json";
        } else {
            // fallback to local file
            p = std::filesystem::path("config.json");
        }
    }
    return p.string();
}