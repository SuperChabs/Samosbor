#pragma once 

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <notcurses/notcurses.h>
#include <vector>
#include <string>

#include "entity.hpp"

class Monster : public Entity 
{
public:
    Monster(int x, int y);

    // Updated AI: receives player's position and the level map to avoid walls
    void Update(int targetX, int targetY, const std::vector<std::wstring>& level) override;
};

#endif