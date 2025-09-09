#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>

// Шаблонний клас менеджера ресурсів
template <typename T>
class ResourceManager {
protected:
    std::unordered_map<std::string, std::shared_ptr<T>> resources;

public:
    virtual ~ResourceManager();

    virtual void Add(const std::string& name, std::shared_ptr<T> resource);
    virtual std::shared_ptr<T> Get(const std::string& name);
    virtual bool Has(const std::string& name) const;
    virtual void Remove(const std::string& name);
    virtual void Clear();
};
