#pragma once 

#ifndef GAME_H
#define GAME_H

#include <notcurses/notcurses.h>
#include <memory>

// * Entitys

#include "player.hpp"
#include "monster.hpp"

// * Managers

#include "inputmanager.hpp"
#include "scenemanager.hpp"

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
    unsigned int rows, cols;

    InputManager input;
    SceneManager sm;

    // bool running;

};

#endif