#pragma once

#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <functional>
#include <memory>

#include "player.hpp"

class Item 
{
public:
    using UseCallback = std::function<void(Player*)>;

private:
    std::string id;         
    std::string name;       
    wchar_t symbol;           
    int maxStack;          
    UseCallback onUse;      
    bool consumable;         

public:
    Item(const std::string& id, const std::string& name, wchar_t symbol,
         int maxStack = 16, bool consumable = true);


    void SetUseCallback(UseCallback callback);

    void Use(Player* player);

    const std::string& GetId() const;
    const std::string& GetName() const;
    wchar_t GetSymbol() const ;
    int GetMaxStack() const;
    bool IsConsumable() const;
};

#endif
