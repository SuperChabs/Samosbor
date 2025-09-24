#include "level1.hpp"

#include <memory>

Level1::Level1(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input)
                : Scene(nc, stdn, rows, cols, input) 
{

}

void Level1::InitEntitys()
{
    player = std::make_shared<Player>(40, 25);
}

void Level1::Draw()
{

}

void Level1::PanelDraw()
{

}
