#pragma once 

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <notcurses/notcurses.h>

#include "entity.hpp"


class Player : public Entity
{
public: 
    Player(int x, int y);
};

#endif