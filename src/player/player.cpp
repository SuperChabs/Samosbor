// #include "player.hpp"

// #include "settings.hpp"

// Player::Player(int x, int y) : Entity(x, y, '#', 0, 255, 0), coins(0), health(100), maxHealth(100), inventory(15) {}

// Player::~Player()
// {
//     coins = 0;
// }

// void Player::ModifyHealth(int amount)
// {
//     health += amount;
//     if (health > maxHealth) health = maxHealth;
//     if (health < 0) health = 0;
// }

// void Player::Move(int dx, int dy, std::vector<std::wstring>& level) 
// {
//     int newX = x + dx;
//     int newY = y + dy;

//     if (newY < 0 || newY >= (int)level.size()) return;
//     if (newX < 0 || newX >= (int)level[0].size()) return;

//     wchar_t target = level[newY][newX];

//     bool isWall = (target == L'━' || target == L'┃' || target == L'┗' ||
//                     target == L'┛' || target == L'┏' || target == L'┓' ||
//                     target == L'░' || target == L'█' || target == L' ');

//     if (!isWall) 
//     {
//         if (target == L'¢') 
//         {
//             Settings::Instance().AddCoins(1);
//             level[newY][newX] = L'.';
//         }

//         x = newX;
//         y = newY;
//     }
// }

// int Player::GetCoinsValue()
// {
//     return Settings::Instance().GetCoins(); 
// }

#include "player.hpp"
#include "settings.hpp"  // ДОДАТИ

Player::Player(int x, int y) : Entity(x, y, '#', 0, 255, 0) 
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
    return Settings::Instance().GetCoins();  // ЗАМІНИТИ return coins на це
}