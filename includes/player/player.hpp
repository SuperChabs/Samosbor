#pragma once 

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <notcurses/notcurses.h>
#include <vector>
#include <string>

#include "entity.hpp"

class Player : public Entity
{
public: 
    Player(int x, int y);
    ~Player();

    void Move(int dx, int dy, std::vector<std::wstring>& level);

    int GetCoinsValue();

private:
    int coins;
};

#endif