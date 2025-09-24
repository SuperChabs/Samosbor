#include "game.hpp"
#include "level1.hpp"

#include <notcurses/notcurses.h>
#include <iostream>
#include <unistd.h>
#include <memory>

Game::Game()
        : running(true)
{
    struct notcurses_options opts = {};
    nc = notcurses_init(&opts, NULL);
    if(!nc) throw std::runtime_error("He вдалося ініціалізувати notcurses");

    stdn = notcurses_stddim_yx(nc, NULL, NULL);
    ncplane_dim_yx(stdn, &rows, &cols);

    scene = std::make_unique<Scene>(nc, stdn, rows, cols, input);

    scenemanager.Add("level1", std::make_shared<Level1>(nc, stdn, rows, cols, input));

    scenemanager.SetActiveScene("level1");
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
        scenemanager.Update();
        scenemanager.Render();
        Update();
    }
}

void Game::Render()
{
    scene->Render();
}

void Game::HandleInput()
{
    input.Bind('q', [&](){ running = false; });
    scene->HandleInput();

}

void Game::Update()
{}

