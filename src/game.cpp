#include "game.hpp"
#include "inputmanager.hpp"

#include <notcurses/notcurses.h>
#include <iostream>
#include <unistd.h>

Game::Game()
        : player(0, 0), monster(0, 0), running(true) 
    {
        struct notcurses_options opts = {};
        nc = notcurses_init(&opts, NULL);
        if(!nc) throw std::runtime_error("Не вдалося ініціалізувати notcurses");

        stdn = notcurses_stddim_yx(nc, NULL, NULL);
        ncplane_dim_yx(stdn, &rows, &cols);

        panelWidth = cols / 4;
        mapWidth   = cols - panelWidth;

        //панель з праву, поотрібно створити свій клас, наслідуючи з класу сцени
        //формально це і є сцена
        struct ncplane_options panel_opts = { .y=0, .x=mapWidth, .rows=rows, .cols=panelWidth };
        panel = ncplane_create(stdn, &panel_opts);

        //перемістити в базовий клас сцени
        struct ncplane_options map_opts = { .y=0, .x=0, .rows=rows, .cols=mapWidth };
        map = ncplane_create(stdn, &map_opts);

        //перенести у базовий клас сцени
        player = Player(mapWidth/2, rows/2);

        //тільки в нащадках базової сцени
        monster = Monster(mapWidth/4, rows/4);
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
    ncplane_erase(map);
    ncplane_erase(panel);

    // фон карти
    for(int y = 0; y < rows; y++) 
    {
        for(int x = 0; x < mapWidth; x++) 
        {
            ncplane_set_fg_rgb8(map, 60, 60, 60);
            ncplane_putstr_yx(map, y, x, ".");
        }
    }


    // малюємо гравця та монстра
    player.Render(map);
    monster.Render(map);

    // панель
    ncplane_set_fg_rgb8(panel, 255, 255, 255);
    ncplane_putstr_yx(panel, 1, 1, "== Панель ==");
    ncplane_putstr_yx(panel, 3, 1, "HP: 100");
    ncplane_putstr_yx(panel, 5, 1, "Інвентар:");
    ncplane_putstr_yx(panel, 6, 3, "- Меч");
    ncplane_putstr_yx(panel, 8, 1, "Коментар:");
    if(!monster.IsAlive())
        ncplane_putstr_yx(panel, 9, 3, "Монстр переможений!");
    else
        ncplane_putstr_yx(panel, 9, 3, "Монстр поруч...");

    notcurses_render(nc);
}

void Game::HandleInput()
{
    input.Bind('q', [&](){ running = false; }); //залишається тут

    //створити та перемістити в клас переміщення гравця. Не знаю що робити монстром
    input.Bind('w', [&](){ player.Move(0, -1, mapWidth, rows); monster.Update(player.GetX(), player.GetY());});
    input.Bind('s', [&](){ player.Move(0,  1, mapWidth, rows); monster.Update(player.GetX(), player.GetY());});
    input.Bind('a', [&](){ player.Move(-1, 0, mapWidth, rows); monster.Update(player.GetX(), player.GetY());});
    input.Bind('d', [&](){ player.Move( 1, 0, mapWidth, rows); monster.Update(player.GetX(), player.GetY());});
    
    //мабуть треба буде модифікувати для зручності
    input.Bind('e', [&](){ 
        if(monster.IsAlive() &&
           abs(player.GetX() - monster.GetX()) + abs(player.GetY() - monster.GetY()) == 1)
        {
            monster.Kill();
        }
    });
}

void Game::Update()
{}

