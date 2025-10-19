#include "menu.hpp"
#include "settings.hpp"  

#include <string>
#include <sstream>
#include <notcurses/notcurses.h>

Menu::Menu(notcurses *nc, ncplane *stdn, unsigned int rows, unsigned int cols, InputManager &input, SceneManager& sm)
    : Scene(nc, stdn, rows, cols, input), selected(0), sm(sm) 
{
    MenuArt = 
    L"  █████████                                             █████                       \n"  
    L" ███▒▒▒▒▒███                                           ▒▒███                        \n"
    L"▒███    ▒▒▒   ██████   █████████████    ██████   █████  ▒███████   ██████  ████████ \n"
    L"▒▒█████████  ▒▒▒▒▒███ ▒▒███▒▒███▒▒███  ███▒▒███ ███▒▒   ▒███▒▒███ ███▒▒███▒▒███▒▒███\n"
    L" ▒▒▒▒▒▒▒▒███  ███████  ▒███ ▒███ ▒███ ▒███ ▒███▒▒█████  ▒███ ▒███▒███ ▒███ ▒███ ▒▒▒ \n"
    L" ███    ▒███ ███▒▒███  ▒███ ▒███ ▒███ ▒███ ▒███ ▒▒▒▒███ ▒███ ▒███▒███ ▒███ ▒███     \n"
    L"▒▒█████████ ▒▒████████ █████▒███ █████▒▒██████  ██████  ████████ ▒▒██████  █████    \n"
    L" ▒▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒ ▒▒▒▒▒  ▒▒▒▒▒▒  ▒▒▒▒▒▒  ▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒  ▒▒▒▒▒     \n";
}

void Menu::Update()
{
    return;
}

void Menu::DrawMap()
{  
    DrawMenuArt();
    
    // ЗМІНИТИ: меню залежить від збереженої гри
    std::vector<std::wstring> items;
    if (Settings::Instance().HasActiveSave()) {
        items = {L"Продовжити", L"Нова гра", L"Вийти"};
    } else {
        items = {L"Грати", L"Вийти"};
    }
    
    int menuY = rows / 2 + 5;
    int menuX = mapWidth / 2 - 5;

    for (int i = 0; i < (int)items.size(); i++) {
        std::wstring& item = items[i];

        if (i == selected) {
            level[menuY + i][menuX - 2] = L'→';
            ncplane_putwc_yx(map, menuY + i, menuX - 2, L'→');
        }

        for (size_t j = 0; j < item.size() && (menuX + j) < mapWidth; ++j) {
            level[menuY + i][menuX + j] = item[j];
            ncplane_putwc_yx(map, menuY + i, menuX + j, item[j]);
        }
    }

    ncplane_set_styles(map, NCSTYLE_NONE);
}

void Menu::PanelDraw()
{
    // Очищаємо панель
    ncplane_set_bg_rgb8(panel, 0, 0, 0);
    unsigned int h, w;
    ncplane_dim_yx(panel, &h, &w);
    
    for (unsigned int y = 0; y < h; ++y) {
        ncplane_putstr_yx(panel, y, 0, std::string(w, ' ').c_str());
    }
    
    // Малюємо декорацію
    ncplane_set_fg_rgb8(panel, 255, 255, 60);
    for (unsigned int i = 0; i < h; i++)
        ncplane_putstr_yx(panel, i, 1, "█");
    
    // ДОДАТИ: показати прогрес якщо є збереження
    if (Settings::Instance().HasActiveSave()) {
        ncplane_set_fg_rgb8(panel, 255, 200, 0);
        std::string coinText = "Монети: " + std::to_string(Settings::Instance().GetCoins());
        ncplane_putstr_yx(panel, 3, 3, coinText.c_str());
        
        std::string levelText = "Рівень: " + std::to_string(Settings::Instance().GetCurrentLevel());
        ncplane_putstr_yx(panel, 5, 3, levelText.c_str());
    }
}

void Menu::HandleInput()
{
    input.Bind('w', [this](){ 
        int maxItems = Settings::Instance().HasActiveSave() ? 3 : 2;
        selected = (selected + maxItems - 1) % maxItems;
    });

    input.Bind('s', [this](){ 
        int maxItems = Settings::Instance().HasActiveSave() ? 3 : 2;
        selected = (selected + 1) % maxItems;
    });

    input.Bind('e', [this](){
        if (Settings::Instance().HasActiveSave()) {
            if (selected == 0) {
                sm.SetActiveScene("level"); 
            } else if (selected == 1) {
                Settings::Instance().ResetProgress();  
                sm.SetActiveScene("level");
            } else if (selected == 2) {
                Settings::Instance().SetRunning(false); 
            }
        } else {
            if (selected == 0) {
                Settings::Instance().ResetProgress();
                sm.SetActiveScene("level"); 
            } else if (selected == 1) {
                Settings::Instance().SetRunning(false); 
            }
        }
    });
}

void Menu::DrawMenuArt()
{
    for (auto &row : level)
        std::fill(row.begin(), row.end(), L' ');

    std::wstringstream wss(MenuArt);
    std::wstring line;
    int artHeight = 0;
    int artWidth = 0;

    {
        std::wstringstream temp(MenuArt);
        std::wstring tmp;
        while (std::getline(temp, tmp)) {
            artHeight++;
            if ((int)tmp.size() > artWidth)
                artWidth = tmp.size();
        }
    }

    int offsetY = (rows - artHeight) / 4;
    int offsetX = (mapWidth - artWidth) / 2;
    
    ncplane_set_fg_rgb8(map, 255, 200, 0);
    
    int y = 0;
    while (std::getline(wss, line) && (y + offsetY) < rows) {
        for (int x = 0; x < (int)line.size() && (x + offsetX) < mapWidth; ++x) {
            if (y + offsetY >= 0 && x + offsetX >= 0)
                level[y + offsetY][x + offsetX] = line[x];
        }
        y++;
    }
}