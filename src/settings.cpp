#include "settings.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

#include <algorithm>
#include <cctype>

void Settings::LoadDefaults()
{
    isRunning = true;
    encodedCoins = EncodeCoins(0);
    currentLevel = 1;
    hasActiveSave = false;
    health = 100;       
    maxHealth = 100;     
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
    SaveToFile("config.json");
}

// Simple helpers for minimal JSON-like parsing used by this small config.
static inline std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace((unsigned char)s[a])) ++a;
    while (b > a && std::isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b-a);
}

static inline bool iequals(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) if (std::tolower((unsigned char)a[i]) != std::tolower((unsigned char)b[i])) return false;
    return true;
}

static std::string extract_json_value(const std::string& content, const std::string& key) {

    std::string quote_key = '"' + key + '"';
    size_t pos = content.find(quote_key);
    if (pos == std::string::npos) return std::string();
    pos = content.find(':', pos + quote_key.size());
    if (pos == std::string::npos) return std::string();
    ++pos;

    while (pos < content.size() && std::isspace((unsigned char)content[pos])) ++pos;
    if (pos >= content.size()) return std::string();
    if (content[pos] == '"') {

        ++pos;
        size_t end = content.find('"', pos);
        if (end == std::string::npos) end = content.size();
        return content.substr(pos, end - pos);
    } else {

        size_t end = pos;
        while (end < content.size() && content[end] != ',' && content[end] != '}' && !std::isspace((unsigned char)content[end])) ++end;
        return content.substr(pos, end - pos);
    }
}

bool Settings::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {

        LoadDefaults();
        SaveToFile(filename.empty() ? std::string("config.json") : filename);
        return true; 
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::string v;

    v = extract_json_value(content, "running");
    if (!v.empty()) isRunning = (iequals(trim(v), "true") || trim(v) == "1");

    v = extract_json_value(content, "coins");
    if (!v.empty()) {
        try { encodedCoins = std::stoi(trim(v)); } catch (...) {}
    }

    v = extract_json_value(content, "level");
    if (!v.empty()) {
        try { currentLevel = std::stoi(trim(v)); } catch (...) {}
    }

    v = extract_json_value(content, "has_save");
    if (!v.empty()) hasActiveSave = (iequals(trim(v), "true") || trim(v) == "1");

    v = extract_json_value(content, "health");
    if (!v.empty()) {
        try { health = std::stoi(trim(v)); } catch (...) {}
    }

    v = extract_json_value(content, "max_health");
    if (!v.empty()) {
        try { maxHealth = std::stoi(trim(v)); } catch (...) {}
    }

    return true;
}

bool Settings::SaveToFile(const std::string& filename)
{
    std::string outname = filename.empty() ? std::string("config.json") : filename;
    std::ofstream file(outname);
    if (!file.is_open()) return false;


    file << "{\n";
    file << "  \"running\": " << (isRunning ? "true" : "false") << ",\n";
    file << "  \"coins\": " << encodedCoins << ",\n";
    file << "  \"level\": " << currentLevel << ",\n";
    file << "  \"health\": " << health << ",\n";
    file << "  \"max_health\": " << maxHealth << ",\n";
    file << "  \"has_save\": " << (hasActiveSave ? "true" : "false") << "\n";
    file << "}\n";

    file.close();
    return true;
}