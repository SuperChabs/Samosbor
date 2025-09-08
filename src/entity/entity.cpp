#include "entity.hpp"

#include <notcurses/notcurses.h>
#include <string>

Entity::Entity(int x, int y, char symbol, unsigned int r, unsigned int g, unsigned int b)
    : x(x), y(y), symbol(symbol), r(r), g(g), b(b), alive(true) {}

void Entity::Render(ncplane *plane)
{
    if (!alive) return;
    ncplane_set_fg_rgb8(plane, r, g, b);
    ncplane_putstr_yx(plane, y, x, std::string(1, symbol).c_str());
}

void Entity::Move(int dx, int dy, int maxX, int maxY)
{
    if (!alive) return;
    int newX = x + dx;
    int newY = y + dy;
    if (newX >= 0 && newX < maxX) x = newX;
    if (newY >= 0 && newY < maxY) y = newY;
}
