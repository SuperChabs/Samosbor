#pragma once
#include <notcurses/notcurses.h>
#include <functional>
#include <unordered_map>

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;


    void Bind(int key, std::function<void()> action);
    void Bind(int key);

    void Update(struct notcurses* nc);
    void UpdateOnce(struct notcurses* nc);

private:
    std::unordered_map<int, std::function<void()>> bindings;
};