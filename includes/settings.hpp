#pragma once

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>

class Settings 
{
private:
    Settings() = default;
    
    // Налаштування гри
    bool isRunning;
    
    // Прогрес гри (закодовано)
    int encodedCoins;
    int currentLevel;
    bool hasActiveSave;
    int health;          // ДОДАТИ
    int maxHealth;       // ДОДАТИ
    
    // XOR ключ для кодування
    static const int XOR_KEY = 0x5A3C;
    
    int EncodeCoins(int coins) const { return coins ^ XOR_KEY; }
    int DecodeCoins(int encoded) const { return encoded ^ XOR_KEY; }

public:
    static Settings& Instance() {
        static Settings instance;
        return instance;
    }
    
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    
    // Геттери
    bool IsRunning() const { return isRunning; }
    int GetCoins() const { return DecodeCoins(encodedCoins); }
    int GetCurrentLevel() const { return currentLevel; }
    bool HasActiveSave() const { return hasActiveSave; }
    int GetHealth() const { return health; }          // ДОДАТИ
    int GetMaxHealth() const { return maxHealth; }    // ДОДАТИ
    
    // Сеттери
    void SetRunning(bool value) { isRunning = value; }
    void SetCoins(int value) { encodedCoins = EncodeCoins(value); }
    void AddCoins(int value) { SetCoins(GetCoins() + value); }
    void SetCurrentLevel(int value) { currentLevel = value; }
    void SetHasActiveSave(bool value) { hasActiveSave = value; }
    void SetHealth(int value) { health = value; }                  // ДОДАТИ
    void ModifyHealth(int amount) {                                 // ДОДАТИ
        health += amount;
        if (health > maxHealth) health = maxHealth;
        if (health < 0) health = 0;
    }
    
    // Робота з прогресом
    void ResetProgress();
    void SaveProgress();
    
    // Завантаження/збереження
    void LoadDefaults();
    bool LoadFromFile(const std::string& filename);
    bool SaveToFile(const std::string& filename);
};

#endif