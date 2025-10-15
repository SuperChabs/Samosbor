#pragma once

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include <memory>
#include <string>
#include <iostream>

class Item;
class Player;

class Inventory {
public:
    struct Slot {
        std::shared_ptr<Item> item;
        int count = 0;
    };

    Inventory(int capacity = 16);
    ~Inventory();

    // Try to add item (stacking when possible). Returns true if added (fully or partially).
    bool AddItem(const std::shared_ptr<Item>& item, int count = 1);

    // Select next/previous slot in UI
    void SelectNext();
    void SelectPrevious();

    // Use currently selected item on player
    void UseSelectedItem(Player* player);

    // For debug / panel drawing
    const std::vector<Slot>& GetSlots() const { return slots; }
    int GetSelectedIndex() const { return selectedIndex; }

private:
    std::vector<Slot> slots;
    int selectedIndex = 0;
    int capacity;
};

#endif
