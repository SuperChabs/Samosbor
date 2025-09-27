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
}

void Level1::DrawMap()
{
    // draw an outlined rectangle (could be a square) using the shared MapUtils helper
    int topY = 20;
    int leftX = 30;
    int width = static_cast<int>(mapWidth) - 5 * leftX; // width between left and right margins
    int height = 8; // example height (20..31 inclusive gives 12 rows)

    MapUtils::DrawRect(level, rows, mapWidth, topY, leftX, width, height, L'@', false);

    for (int x = 0; x < mapWidth; x++) 
    {
        level[0][x] = L'\u2581';
        level[rows-1][x] = L'\u2581'; 
    }

    for (int y = 0; y < rows; y++) 
    {
        level[y][0] = L'\u258F';          
        level[y][mapWidth-1] = L'\u258F'; 
    }
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