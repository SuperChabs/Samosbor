#include "player.hpp"

Player::Player(int x, int y) : Entity(x, y, '#', 0, 255, 0) {}

Player::~Player()
{
    coins = 0;
}

void Player::Move(int dx, int dy, std::vector<std::wstring>& level) 
{
    int newX = x + dx;
    int newY = y + dy;

    if (newY < 0 || newY >= (int)level.size()) return;
    if (newX < 0 || newX >= (int)level[0].size()) return;

    wchar_t target = level[newY][newX];

    // Якщо символ прохідний або вихід
    if (target == '.' || target == L'^' || target == L'v' || target == L'¢') 
    {
        // збираємо монетку
        if (target == L'¢') {
            coins++;
            level[newY][newX] = L'.'; // видаляємо монетку з карти
        }

        // рухаємо гравця
        x = newX;
        y = newY;
    }
}

int Player::GetCoinsValue()
{
    return coins;
}
