#pragma once

#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <functional>
#include <memory>

// Forward declaration
class Player;

class Item {
public:
    using UseCallback = std::function<void(Player*)>;

private:
    std::string id;           // unique id
    std::string name;         // display name
    wchar_t symbol;           // symbol for rendering
    int maxStack;             // maximum stack size
    UseCallback onUse;        // callback when used
    bool consumable;          // whether it's consumed on use

public:
    Item(const std::string& id, 
         const std::string& name, 
         wchar_t symbol,
         int maxStack = 16,
         bool consumable = true)
        : id(id), name(name), symbol(symbol), maxStack(maxStack), consumable(consumable), onUse(nullptr) {}

    void SetUseCallback(UseCallback callback) { onUse = callback; }

    void Use(Player* player) {
        if (onUse) {
            onUse(player);
        }
    }

    const std::string& GetId() const { return id; }
    const std::string& GetName() const { return name; }
    wchar_t GetSymbol() const { return symbol; }
    int GetMaxStack() const { return maxStack; }
    bool IsConsumable() const { return consumable; }
};

#endif
