#include "settings.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

void Settings::LoadDefaults()
{
    isRunning = true;
    encodedCoins = EncodeCoins(0);
    currentLevel = 1;
    hasActiveSave = false;
    health = 100;        // ДОДАТИ
    maxHealth = 100;     // ДОДАТИ
}

void Settings::ResetProgress()
{
    encodedCoins = EncodeCoins(0);
    currentLevel = 1;
    hasActiveSave = false;
    health = 100;        // ДОДАТИ
    maxHealth = 100;     // ДОДАТИ
}

void Settings::SaveProgress()
{
    hasActiveSave = true;
    SaveToFile("config.txt");
}

bool Settings::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        LoadDefaults();
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string key, equals, value;
        
        if (!(iss >> key >> equals)) continue;
        if (equals != "=") continue;
        
        std::getline(iss >> std::ws, value);
        
        if (key == "running") isRunning = (value == "true" || value == "1");
        else if (key == "coins") encodedCoins = std::stoi(value);
        else if (key == "level") currentLevel = std::stoi(value);
        else if (key == "has_save") hasActiveSave = (value == "true" || value == "1");
        else if (key == "health") health = std::stoi(value);          // ДОДАТИ
        else if (key == "max_health") maxHealth = std::stoi(value);   // ДОДАТИ
    }
    
    file.close();
    return true;
}

bool Settings::SaveToFile(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    file << "# Samosbor Settings\n\n";
    file << "running = " << (isRunning ? "true" : "false") << "\n\n";
    
    file << "# Progress\n";
    file << "coins = " << encodedCoins << "\n";
    file << "level = " << currentLevel << "\n";
    file << "health = " << health << "\n";              // ДОДАТИ
    file << "max_health = " << maxHealth << "\n";       // ДОДАТИ
    file << "has_save = " << (hasActiveSave ? "true" : "false") << "\n";
    
    file.close();
    return true;
}