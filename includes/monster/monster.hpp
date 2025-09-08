#pragma once 

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <notcurses/notcurses.h>

#include "entity.hpp"

class Monster : public Entity 
{
public:
    Monster(int x, int y);

    void Update(int targetX, int targetY) override;
};

#endif