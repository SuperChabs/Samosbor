#include "event.hpp"

void EventManager::Subscribe(const std::string& event, Callback cb) 
{
    listeners[event].push_back(cb);
}

void EventManager::Emit(const std::string& event) 
{
    if (listeners.count(event)) 
    {
        for (auto& cb : listeners[event]) 
        {
            cb();
        }
    }
}