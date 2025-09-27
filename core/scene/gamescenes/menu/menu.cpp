#include "menu.hpp"
#include <notcurses/notcurses.h>

MenuScene::MenuScene(notcurses* nc, ncplane* stdn, unsigned int rows, unsigned int cols,
                     InputManager& input, SceneManager& sm, bool& running)
    : Scene(nc, stdn, rows, cols, input), scenemanager(sm), running(running), selected(0)
{
    options.push_back("Грати рівень 1");
    options.push_back("Вийти з гри");

    SetupInput();
}

MenuScene::~MenuScene(){}

void MenuScene::DrawMap() 
{
    // меню не малює карту
}

void MenuScene::PanelDraw() 
{
    if(!panel) return;

    ncplane_erase(panel);
    ncplane_set_fg_rgb8(panel, 255, 255, 255);
    ncplane_putstr_yx(panel, 1, 1, "=== МЕНЮ ===");

    for(size_t i = 0; i < options.size(); ++i) 
    {
        std::string text = (i == selected ? "> " : "  ") + options[i];
        ncplane_set_fg_rgb8(panel, i == selected ? 200 : 255, i == selected ? 50 : 255, i == selected ? 50 : 255);
        ncplane_putstr_yx(panel, 3 + i, 3, text.c_str());
    }
}

void MenuScene::SetupInput() 
{
    input.Bind('w', [this](){
        selected = (selected - 1 + options.size()) % options.size();
    });
    input.Bind('s', [this](){
        selected = (selected + 1) % options.size();
    });

    auto enterHandler = [this](){
        if(selected >= options.size()) return;

        if(options[selected] == "Грати рівень 1") 
        {
            if(scenemanager.Has("level1"))
                scenemanager.SetActiveScene("level1");
        } 
        else if(options[selected] == "Вийти з гри") 
        {
            running = false;
        }
    };
    input.Bind('\n', enterHandler);
    input.Bind('\r', enterHandler);
}
