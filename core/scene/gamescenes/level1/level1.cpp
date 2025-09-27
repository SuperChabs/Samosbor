#include "level1.hpp"

#include <memory>

Level1::Level1(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input)
                : Scene(nc, stdn, rows, cols, input) 
{
    // panelWidth = cols / 4;
    // mapWidth   = cols - panelWidth;

    // struct ncplane_options panel_opts = 
    // { 
    //     .y=0, 
    //     .x=(int)mapWidth, 
    //     .rows=rows, 
    //     .cols=panelWidth 
    // };
    // panel = ncplane_create(stdn, &panel_opts);

    // struct ncplane_options map_opts = 
    // {
    //      .y=0, 
    //      .x=0, 
    //      .rows=rows, 
    //      .cols=mapWidth 
    // };
    // map = ncplane_create(stdn, &map_opts);

    InitEntitys();

    level.resize(rows, std::wstring(mapWidth, L'.'));
}

void Level1::InitEntitys()
{
    player = std::make_shared<Player>(1, 1);
}

void Level1::DrawMap()
{
    // for(int y = 28; y < 29; y++) {
    //     for(int x = 0; x < mapWidth; x++) {
    //         level[y][x] = L'@';
    //     }
    // }
}

void Level1::PanelDraw()
{
    ncplane_set_fg_rgb8(panel, 255, 255, 255);
    ncplane_putstr_yx(panel, 1, 1, "Bro`s typing shit😂😂😂");
}

void Level1::Update(ncplane *map)
{
    player->Render(map);
}
