#pragma once

#ifndef EVENT_HPP
#define EVENT_HPP

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>

class EventManager {
public:
    using Callback = std::function<void()>;  // функція без аргументів

    void Subscribe(const std::string& event, Callback cb); 
    void Emit(const std::string& event); 

private:
    std::unordered_map<std::string, std::vector<Callback>> listeners;
};

#endif