#include "player.hpp"

Player::Player(int x, int y) : Entity(x, y, '#', 0, 255, 0) {}

void Player::Move(int dx, int dy, const std::vector<std::wstring>& level) {
    int newX = x + dx;
    int newY = y + dy;

    if (newY < 0 || newY >= (int)level.size()) return;
    if (newX < 0 || newX >= (int)level[0].size()) return;

    // Якщо символ прохідний
    if (level[newY][newX] == '.') {
        x = newX;
        y = newY;
    }
}