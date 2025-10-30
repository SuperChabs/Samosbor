#pragma once

#ifndef ITEMREGISTRY_HPP
#define ITEMREGISTRY_HPP

#include "item.hpp"
#include "panelmanager.hpp"
#include "settings.hpp"

#include <unordered_map>
#include <memory>
#include <string>

class ItemRegistry 
{
private:
    static std::unordered_map<std::string, std::shared_ptr<Item>> items;

    static PanelManager& panel;

public:
    static void RegisterItem(std::shared_ptr<Item> item);

    static std::shared_ptr<Item> GetItem(const std::string& id); 
    static const std::unordered_map<std::string, std::shared_ptr<Item>>& GetAllItems();

    static void InitializeItems(PanelManager& panelManager);
};

#endif
