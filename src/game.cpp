#include "game.hpp"

#include <notcurses/notcurses.h>
#include <iostream>
#include <unistd.h>
#include <memory>

#include "gamescene.hpp"
#include "menu.hpp"
#include "../includes/settings.hpp"

Game::Game()
    {
        // load settings
        Settings::Instance().LoadFromFile("./json/settings.json");

        struct notcurses_options opts = {};
        nc = notcurses_init(&opts, NULL);
        if(!nc) throw std::runtime_error("Не вдалося ініціалізувати notcurses");

        stdn = notcurses_stddim_yx(nc, NULL, NULL);
        ncplane_dim_yx(stdn, &rows, &cols);

        sm.Add("level", std::make_shared<GameScene>(nc, stdn, rows, cols, input));
        sm.Add("menu", std::make_shared<Menu>(nc, stdn, rows, cols, input, sm));

        sm.SetActiveScene("level");

    }

Game::~Game(void)
{
    Settings::Instance().SaveProgress();  // ДОДАТИ
    
    if(nc) 
    {
        notcurses_stop(nc);
    }
}

void Game::Run(void)
{
    HandleInput();

    while(Settings::Instance().IsRunning()) 
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
    input.Bind('q', [&](){Settings::Instance().SetRunning(false);});
    sm.HandleInput();

}

void Game::Update()
{
    //sm.Update();
}

