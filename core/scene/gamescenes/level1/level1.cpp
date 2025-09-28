#include "level1.hpp"

#include <memory>
#include "../../map_utils.hpp"

Level1::Level1(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input)
                : Scene(nc, stdn, rows, cols, input) 
{
    InitEntitys();

    level.resize(rows, std::wstring(mapWidth, L'.'));
}

void Level1::InitEntitys()
{
    player = std::make_shared<Player>(36, 23);
    monsters.push_back(std::make_shared<Monster>(10, 10));
}

void Level1::DrawMap()
{
    MapUtils::DrawRect(level, rows, mapWidth, 20, 30, static_cast<int>(mapWidth) - 5 * 30, 9, L'░', false);
    level[24][30] = '.';

    MapUtils::DrawRect(level, rows, mapWidth, 18, 58, static_cast<int>(mapWidth) - 5 * 30, 13, L'░', false);
    level[24][58] = '.';
    
    for (int x = 0; x < mapWidth; x++) 
    {
        level[0][x] = L'\u2501';
        level[rows-1][x] = L'\u2501'; 
    }

    for (int y = 0; y < rows; y++) 
    {
        level[y][0] = L'\u2503';          
        level[y][mapWidth-1] = L'\u2503'; 
    }

    level[rows - 1][0] = L'\u2517';
    level[rows - 1][mapWidth - 1] = L'\u251B';
    level[0][0] = L'\u250F';
    level[0][mapWidth - 1] = L'\u2513';
}

void Level1::PanelDraw()
{
    ncplane_set_fg_rgb8(panel, 255, 0, 255);
    ncplane_putstr_yx(panel, 1, 1, "Bro just typing shit😂😂😂");
}

void Level1::Update(ncplane *map)
{
    player->Render(map);
    for (auto monster : monsters) 
    {
        monster->Render(map);
    }
}