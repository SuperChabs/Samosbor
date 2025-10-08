#include "menu.hpp"

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

void Menu::Update(ncplane *nc)
{
    return;
}

void Menu::DrawMap()
{  
    DrawMenuArt();
    std::vector<std::wstring> items = {L"Грати", L"Вийти"};
    
    int menuY = rows / 2 + 5;      // трохи нижче ASCII-арту
    int menuX = mapWidth / 2 - 5;  // приблизно по центру

    for (int i = 0; i < (int)items.size(); i++) {
        std::wstring& item = items[i];

        // якщо активний пункт, малюємо стрілку
        if (i == selected) {
            level[menuY + i][menuX - 2] = L'→';          // стрілка у level
            ncplane_putwc_yx(map, menuY + i, menuX - 2, L'→'); // і на ncplane
        }

        // малюємо текст пункту
        for (size_t j = 0; j < item.size() && (menuX + j) < mapWidth; ++j) {
            level[menuY + i][menuX + j] = item[j];
            ncplane_putwc_yx(map, menuY + i, menuX + j, item[j]);
        }
    };

    ncplane_set_styles(map, NCSTYLE_NONE);
}

void Menu::PanelDraw()
{
    ncplane_set_fg_rgb8(panel, 255, 255, 60);

    unsigned int h, w;
    ncplane_dim_yx(panel, &h, &w); 

    for (int i = 0; i < h; i++)
        ncplane_putstr_yx(panel, i, 1, "█");
}

void Menu::HandleInput()
{
	input.Bind('w', [this](){ 
        selected = (selected + 2 - 1) % 2;
	});

	input.Bind('s', [this](){ 
        selected = (selected + 1) % 2;
	});

    input.Bind('e', [this](){
        if(selected == 0){
            sm.SetActiveScene("level"); 
        } else if(selected == 1){
            notcurses_stop(nc); // завершуємо гру
        }
    });
}

void Menu::DrawMenuArt()
{
    // Очищаємо карту
    for (auto &row : level)
        std::fill(row.begin(), row.end(), L' ');

    // Готуємо читання з MenuArt
    std::wstringstream wss(MenuArt);
    std::wstring line;
    int artHeight = 0;
    int artWidth = 0;

    // Спочатку визначаємо розміри арту
    {
        std::wstringstream temp(MenuArt);
        std::wstring tmp;
        while (std::getline(temp, tmp)) {
            artHeight++;
            if ((int)tmp.size() > artWidth)
                artWidth = tmp.size();
        }
    }

    // Обчислюємо офсети для центрування
    int offsetY = (rows - artHeight) / 4;  // трохи нижче "стелі" (1/4 від верху)
    int offsetX = (mapWidth - artWidth) / 2; // по центру по горизонталі

    // Записуємо арт у level з офсетами
    int y = 0;
    while (std::getline(wss, line) && (y + offsetY) < rows) {
        for (int x = 0; x < (int)line.size() && (x + offsetX) < mapWidth; ++x) {
            if (y + offsetY >= 0 && x + offsetX >= 0)
                level[y + offsetY][x + offsetX] = line[x];
        }
        y++;
    }
}
