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

    void Move(int dx, int dy, const std::vector<std::wstring>& level);
};

#endif