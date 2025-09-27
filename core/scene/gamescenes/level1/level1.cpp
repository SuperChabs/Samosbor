#include "level1.hpp"

#include <memory>

Level1::Level1(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input)
                : Scene(nc, stdn, rows, cols, input) 
{
    InitEntitys();

    level.resize(rows, std::wstring(mapWidth, L'.'));
}

void Level1::InitEntitys()
{
    player = std::make_shared<Player>(1, 1);
}

void Level1::DrawMap()
{

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