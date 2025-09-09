#include "resourcemanager.hpp"

// Деструктор
template <typename T>
ResourceManager<T>::~ResourceManager() = default;

// Додати ресурс
template <typename T>
void ResourceManager<T>::Add(const std::string& name, std::shared_ptr<T> resource) {
    resources[name] = resource;
}

// Отримати ресурс
template <typename T>
std::shared_ptr<T> ResourceManager<T>::Get(const std::string& name) {
    if (!Has(name)) {
        throw std::runtime_error("Resource '" + name + "' not found");
    }
    return resources[name];
}

// Перевірити існування
template <typename T>
bool ResourceManager<T>::Has(const std::string& name) const {
    return resources.find(name) != resources.end();
}

// Видалити ресурс
template <typename T>
void ResourceManager<T>::Remove(const std::string& name) {
    resources.erase(name);
}

// Очистити всі ресурси
template <typename T>
void ResourceManager<T>::Clear() {
    resources.clear();
}
