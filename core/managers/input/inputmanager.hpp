#pragma once
#include <notcurses/notcurses.h>
#include <functional>
#include <unordered_map>

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    // Призначаємо функцію на конкретну клавішу
    void Bind(int key, std::function<void()> action);

    // Читаємо введення і викликаємо функції
    void Update(struct notcurses* nc);
    void UpdateOnce(struct notcurses* nc);

private:
    std::unordered_map<int, std::function<void()>> bindings;
};