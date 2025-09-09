#pragma once 

#ifndef GAME_H
#define GAME_H

#include <notcurses/notcurses.h>

#include "player.hpp"
#include "monster.hpp"
#include "inputmanager.hpp"

class Game 
{
public:
    Game();
    ~Game();

    void Run();

private:
    void Render();
    void HandleInput(); 
    void Update();

private:
    struct notcurses* nc;
    struct ncplane* stdn;
    struct ncplane* map;
    struct ncplane* panel;
    unsigned int rows, cols;
    unsigned int mapWidth, panelWidth;

    Player player;
    Monster monster;
    InputManager input;

    bool running;

};

#endif