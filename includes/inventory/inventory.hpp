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

    bool AddItem(const std::shared_ptr<Item>& item, int count = 1);

    void SelectNext();
    void SelectPrevious();

    void UseSelectedItem(Player* player);

    const std::vector<Slot>& GetSlots() const { return slots; }
    int GetSelectedIndex() const { return selectedIndex; }

private:
    std::vector<Slot> slots;
    int selectedIndex = 0;
    int capacity;
};

#endif
