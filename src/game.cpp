#include "game.hpp"
#include "level1.hpp"
#include "menu.hpp"

#include <notcurses/notcurses.h>
#include <iostream>
#include <unistd.h>
#include <memory>
#include <thread>
#include <chrono>

Game::Game()
        : running(true)
{
    struct notcurses_options opts = {};
    nc = notcurses_init(&opts, NULL);
    if(!nc) throw std::runtime_error("He вдалося ініціалізувати notcurses");

    stdn = notcurses_stddim_yx(nc, NULL, NULL);
    ncplane_dim_yx(stdn, &rows, &cols);

    //scene = std::make_unique<Scene>(nc, stdn, rows, cols, input);

    scenemanager.Add("level1", std::make_shared<Level1>(nc, stdn, rows, cols, input));
    scenemanager.Add("menu", std::make_shared<MenuScene>(nc, stdn, rows, cols, input, scenemanager, running));

    scenemanager.SetActiveScene("level1");
}

Game::~Game(void)
{
    if(nc) 
    {
        //scenemanager.Clear();
        notcurses_stop(nc);
    }
}

void Game::Run(void)
{
    HandleInput();

    while(running) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        input.UpdateOnce(nc);
        HandleInput();
        Render();
    }
}

void Game::Render()
{
    //scene->Render();
    scenemanager.Render();
}

void Game::HandleInput()
{
    input.Bind('q', [&](){ running = false; });
    scenemanager.HandleInput();

}

void Game::Update()
{}

