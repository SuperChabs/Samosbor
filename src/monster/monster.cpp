#include "monster.hpp"

Monster::Monster(int x, int y) : Entity(x, y, 'M', 200, 0, 0) {}

void Monster::Update(int targetX, int targetY)
{
    if (!alive) return;
    if (x < targetX) x++;
    else if (x > targetX) x--;
    if (y < targetY) y++;
    else if (y > targetY) y--;
}