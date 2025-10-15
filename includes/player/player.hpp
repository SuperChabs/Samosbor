#pragma once 

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <notcurses/notcurses.h>
#include <vector>
#include <string>

#include "entity.hpp"
#include "inventory/inventory.hpp"

class Player : public Entity
{
public: 
    Player(int x, int y);
    ~Player();

    void Move(int dx, int dy, std::vector<std::wstring>& level);

    int GetCoinsValue();
    int GetHealth() const;
    int GetMaxHealth() const;

    void ModifyHealth(int amount);

    Inventory& GetInventory() { return inventory; }

private:
    Inventory inventory{16};

};

#endif