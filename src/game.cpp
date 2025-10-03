#include "game.hpp"

#include <notcurses/notcurses.h>
#include <iostream>
#include <unistd.h>
#include <memory>

#include "gamescene.hpp"

Game::Game()
        : running(true)
    {
        struct notcurses_options opts = {};
        nc = notcurses_init(&opts, NULL);
        if(!nc) throw std::runtime_error("Не вдалося ініціалізувати notcurses");

        stdn = notcurses_stddim_yx(nc, NULL, NULL);
        ncplane_dim_yx(stdn, &rows, &cols);

        sm.Add("level", std::make_shared<GameScene>(nc, stdn, rows, cols, input));

        sm.SetActiveScene("level");

    }

Game::~Game(void)
{
    if(nc) 
    {
        notcurses_stop(nc);
    }
}

void Game::Run(void)
{
    HandleInput();

    while(running) 
    {
        Render();
        input.UpdateOnce(nc);
        Update();
    }
}

void Game::Render()
{
    sm.Render();
}

void Game::HandleInput()
{
    input.Bind('q', [&](){ running = false; });
    sm.HandleInput();

}

void Game::Update()
{}

