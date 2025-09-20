#pragma once 

#ifndef GAME_H
#define GAME_H

#include <notcurses/notcurses.h>
#include <memory>

#include "player.hpp"
#include "monster.hpp"
#include "inputmanager.hpp"
#include "scene.hpp"

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

    std::unique_ptr<Scene> scene;

    bool running;

};

#endif