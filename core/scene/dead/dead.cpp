#include "dead.hpp"
#include "settings.hpp"  

#include <string>
#include <sstream>
#include <notcurses/notcurses.h>

Dead::Dead(notcurses *nc, ncplane *stdn, unsigned int rows, unsigned int cols, InputManager &input, SceneManager& sm)
    : Menu(nc, stdn, rows, cols, input, sm)
{
    MenuArt = 
        LR"(▓██   ██▓ ▒█████   █    ██    ▓█████▄ ▓█████ ▄▄▄      ▓█████▄  
             ▒██  ██▒▒██▒  ██▒ ██  ▓██▒   ▒██▀ ██▌▓█   ▀▒████▄    ▒██▀ ██▌ 
              ▒██ ██░▒██░  ██▒▓██  ▒██░   ░██   █▌▒███  ▒██  ▀█▄  ░██   █▌ 
              ░ ▐██▓░▒██   ██░▓▓█  ░██░   ░▓█▄   ▌▒▓█  ▄░██▄▄▄▄██ ░▓█▄   ▌ 
              ░ ██▒▓░░ ████▓▒░▒▒█████▓    ░▒████▓ ░▒████▒▓█   ▓██▒░▒████▓  
               ██▒▒▒ ░ ▒░▒░▒░ ░▒▓▒ ▒ ▒     ▒▒▓  ▒ ░░ ▒░ ░▒▒   ▓▒█░ ▒▒▓  ▒  
             ▓██ ░▒░   ░ ▒ ▒░ ░░▒░ ░ ░     ░ ▒  ▒  ░ ░  ░ ▒   ▒▒ ░ ░ ▒  ▒  
             ▒ ▒ ░░  ░ ░ ░ ▒   ░░░ ░ ░     ░ ░  ░    ░    ░   ▒    ░ ░  ░  
             ░ ░         ░ ░     ░           ░       ░  ░     ░  ░   ░     
             ░ ░                           ░                       ░      )";
}