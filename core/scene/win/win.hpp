#pragma once 

#ifndef WIN_HPP
#define WIN_HPP

#include "menu.hpp"

#include <notcurses/notcurses.h>
#include <string>

#include "scenemanager.hpp"

class Win : public Menu 
{
public:
    Win(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, 
        InputManager& input, SceneManager& sm);

}; 

#endif