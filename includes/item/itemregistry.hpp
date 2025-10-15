#pragma once

#ifndef ITEMREGISTRY_HPP
#define ITEMREGISTRY_HPP

#include "item.hpp"
#include <unordered_map>
#include <memory>
#include <string>

class ItemRegistry {
private:
    static std::unordered_map<std::string, std::shared_ptr<Item>> items;

public:
    static void RegisterItem(std::shared_ptr<Item> item) {
        if (item) {
            items[item->GetId()] = item;
        }
    }

    static std::shared_ptr<Item> GetItem(const std::string& id) {
        auto it = items.find(id);
        if (it != items.end()) {
            return it->second;
        }
        return nullptr;
    }

    static const std::unordered_map<std::string, std::shared_ptr<Item>>& GetAllItems() {
        return items;
    }

    static void InitializeItems();
};

#endif
