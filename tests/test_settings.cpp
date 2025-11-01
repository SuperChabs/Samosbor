#include <iostream>
#include "settings.hpp"

int main() {
    auto &s = Settings::Instance();

    std::cout << "Defaults: running=" << s.IsRunning()
              << " coins=" << s.GetCoins()
              << " level=" << s.GetCurrentLevel()
              << " health=" << s.GetHealth()
              << " max_health=" << s.GetMaxHealth()
              << " has_save=" << s.HasActiveSave() << std::endl;

    // Change values
    s.SetCoins(123);
    s.SetCurrentLevel(5);
    s.SetHealth(42);
    s.SetHasActiveSave(true);
    s.SetRunning(false);

    const std::string filename = "test_config.json";
    if (!s.SaveToFile(filename)) {
        std::cerr << "SaveToFile failed\n";
        return 1;
    }
    std::cout << "Saved to " << filename << std::endl;

    // Reset to different values to ensure load overwrites
    s.SetCoins(0);
    s.SetCurrentLevel(1);
    s.SetHealth(1);
    s.SetHasActiveSave(false);
    s.SetRunning(true);

    if (!s.LoadFromFile(filename)) {
        std::cerr << "LoadFromFile failed\n";
        return 2;
    }

    std::cout << "After load: running=" << s.IsRunning()
              << " coins=" << s.GetCoins()
              << " level=" << s.GetCurrentLevel()
              << " health=" << s.GetHealth()
              << " max_health=" << s.GetMaxHealth()
              << " has_save=" << s.HasActiveSave() << std::endl;

    return 0;
}
