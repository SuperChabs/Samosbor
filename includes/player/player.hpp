#pragma once 

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <notcurses/notcurses.h>

#include "entity.hpp"
#include "eventmanager.hpp"

class PlayerMoveEvent : public Event {};

class Player : public Entity
{
public: 
    Player(int x, int y);
};

#endif