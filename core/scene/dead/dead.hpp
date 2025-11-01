#pragma once 

#ifndef DEAD_HPP
#define DEAD_HPP

#include "menu.hpp"

#include <notcurses/notcurses.h>
#include <string>

#include "scenemanager.hpp"

class Dead : public Menu
{
public:
    Dead(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, 
        InputManager& input, SceneManager& sm);
}; 

#endif
