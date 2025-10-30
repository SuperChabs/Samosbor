#include "monster.hpp"
#include <cstdlib>
#include <algorithm>

Monster::Monster(int x, int y) : Entity(x, y, 'M', 200, 0, 0) {}

static bool HasLineOfSight(int sx, int sy, int tx, int ty, const std::vector<std::wstring>& level)
{
    int deltaX = tx - sx;
    int deltaY = ty - sy;
    int absDx = std::abs(deltaX);
    int absDy = std::abs(deltaY);
    int signX = deltaX > 0 ? 1 : (deltaX < 0 ? -1 : 0);
    int signY = deltaY > 0 ? 1 : (deltaY < 0 ? -1 : 0);

    int x = sx;
    int y = sy;

    if (absDx > absDy) {
        int err = absDx / 2;
        for (int i = 0; i < absDx; ++i) 
        {
            x += signX;
            err += absDy;
            if (err >= absDx) 
                err -= absDx, y += signY; 

            if (y >= 0 && y < (int)level.size() && x >= 0 && x < (int)level[0].size()) 
                if (level[y][x] != L'.') return false; 
            else return false; 
        }
    } else 
    {
        int err = absDy / 2;
        for (int i = 0; i < absDy; ++i) 
        {
            y += signY;
            err += absDx;
            if (err >= absDy) err -= absDy, x += signX; 
            if (y >= 0 && y < (int)level.size() && x >= 0 && x < (int)level[0].size()) 
                if (level[y][x] != L'.' || level[y][x] != L'^' || level[y][x] != L'v' || level[y][x] != L' ') return false;
            else return false;
        }
    }
    return true; 
}


static bool TryMove(int &x, int &y, int newX, int newY, const std::vector<std::wstring>& level)
{
    if (newY < 0 || newY >= (int)level.size() || newX < 0 || newX >= (int)level[0].size()) return false;
    
    if (level[newY][newX] == L'.') 
    { 
        x = newX, y = newY; 
        return true; 
    }
    return false; 
}

void Monster::Update(int targetX, int targetY, const std::vector<std::wstring>& level)
{
    if (!alive) return; 
    if (HasLineOfSight(x, y, targetX, targetY, level)) 
    {
        int deltaX = (targetX > x) ? 1 : ((targetX < x) ? -1 : 0);
        int deltaY = (targetY > y) ? 1 : ((targetY < y) ? -1 : 0);

        if (deltaX != 0 && deltaY != 0) 
            if (TryMove(x, y, x + deltaX, y + deltaY, level)) return;
    
        if (deltaX != 0 && TryMove(x, y, x + deltaX, y, level)) return;
        if (deltaY != 0 && TryMove(x, y, x, y + deltaY, level)) return;

        int backX = x - deltaX * 2;
        int backY = y - deltaY * 2;

        if (TryMove(x, y, backX, backY, level)) return;
    }

    
    int directions[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    
    for (int i = 0; i < 4; ++i) 
    {
        int j = std::rand() % 4; 
        std::swap(directions[i], directions[j]);
    }

    for (int i = 0; i < 4; ++i) {
        int candidateX = x + directions[i][0];
        int candidateY = y + directions[i][1];

        if (TryMove(x, y, candidateX, candidateY, level)) return; 

        int oppX = x - directions[i][0] * 2;
        int oppY = y - directions[i][1] * 2;

        if (TryMove(x, y, oppX, oppY, level)) return;
    }
}