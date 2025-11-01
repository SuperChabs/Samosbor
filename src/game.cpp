#include "game.hpp"

#include <notcurses/notcurses.h>
#include <iostream>
#include <unistd.h>
#include <memory>

#include "gamescene.hpp"
#include "menu.hpp"
#include "dead.hpp"
#include "win.hpp"

#include "settings.hpp"
#include "itemregistry.hpp"
#include "panelmanager.hpp"

Game::Game()
    {
        //Settings::Instance().LoadFromFile("./json/settings.json");
        Settings::Instance().LoadDefaults();

        struct notcurses_options opts = {};
        nc = notcurses_init(&opts, NULL);
        if(!nc) throw std::runtime_error("Не вдалося ініціалізувати notcurses");

        stdn = notcurses_stdplane(nc); 
        ncplane_dim_yx(stdn, &rows, &cols);

        ItemRegistry::InitializeItems(panel);

        sm.Add("level", std::make_shared<GameScene>(nc, stdn, rows, cols, input, panel, sm));
        sm.Add("menu", std::make_shared<Menu>(nc, stdn, rows, cols, input, sm));
        sm.Add("dead", std::make_shared<Dead>(nc, stdn, rows, cols, input, sm));
        sm.Add("win", std::make_shared<Win>(nc, stdn, rows, cols, input, sm));

        sm.SetActiveScene("menu");

    }

Game::~Game(void)
{
    Settings::Instance().SaveProgress(); 
    
    if(Settings::Instance().IsRunning() == false) 
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

