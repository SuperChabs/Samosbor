#include "game.hpp"

#include <iostream>

int main () 
{
    try {
        Game game;
        game.Run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
