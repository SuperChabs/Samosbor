#pragma once

#include <notcurses/notcurses.h>
#include <memory>

#include "inputmanager.hpp"
#include "player.hpp"
#include "monster.hpp"

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    void HandleInput();

    struct notcurses* nc = nullptr;
    struct ncplane* stdn = nullptr;

    unsigned int rows = 0;
    unsigned int cols = 0;
    unsigned int mapWidth = 0;
    unsigned int panelWidth = 0;

    bool running = true;

    InputManager input;

    Player player;
    Monster monster;
};
