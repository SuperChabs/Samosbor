#include "player.hpp"
#include "settings.hpp"  // ДОДАТИ

Player::Player(int x, int y) : Entity(x, y, '#', 0, 255, 0), inventory(16)
{
}

Player::~Player()
{
}

void Player::Move(int dx, int dy, std::vector<std::wstring>& level) 
{
     int newX = x + dx;
     int newY = y + dy;

     if (newY < 0 || newY >= (int)level.size()) return;
     if (newX < 0 || newX >= (int)level[0].size()) return;

     wchar_t target = level[newY][newX];

     bool isWall = (target == L'━' || target == L'┃' || target == L'┗' ||
                     target == L'┛' || target == L'┏' || target == L'┓' ||
                     target == L'░' || target == L'█' || target == L' ');

     if (!isWall) 
     {
         if (target == L'¢') 
         {
             Settings::Instance().AddCoins(1);
             level[newY][newX] = L'.';
         }

         x = newX;
         y = newY;
     }
}

int Player::GetCoinsValue()
{
    return Settings::Instance().GetCoins();  
}

void Player::ModifyHealth(int amount)
{
    Settings::Instance().ModifyHealth(amount);  
}

int Player::GetHealth() const
{
    return Settings::Instance().GetHealth();  
}

int Player::GetMaxHealth() const
{
    return Settings::Instance().GetMaxHealth();  
}