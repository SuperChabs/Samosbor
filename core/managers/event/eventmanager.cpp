#include "eventmanager.hpp"

// Template реалізації в header файлі
template<typename T>
void EventManager::Subscribe(std::function<void(const T&)> callback) {
    auto typeIndex = std::type_index(typeid(T));
    
    // Конвертуємо typed callback в generic callback
    auto genericCallback = [callback](const Event& event) {
        callback(static_cast<const T&>(event));
    };
    
    listeners[typeIndex].push_back(genericCallback);
}

template<typename T>
void EventManager::Publish(const T& event) {
    auto typeIndex = std::type_index(typeid(T));
    
    auto it = listeners.find(typeIndex);
    if (it != listeners.end()) {
        for (auto& callback : it->second) {
            callback(event);
        }
    }
}