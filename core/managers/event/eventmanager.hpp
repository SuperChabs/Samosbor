#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>

class Event {

public:
    virtual ~Event() = default;
};

class PlayerMovedEvent : public Event {
public:
    int x, y;
    PlayerMovedEvent(int x, int y) : x(x), y(y) {}
};

class EventManager {
private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> listeners;

public:
    template<typename T>
    void Subscribe(std::function<void(const T&)> callback);
    
    template<typename T>
    void Publish(const T& event);
};

#endif // EVENT_SYSTEM_H