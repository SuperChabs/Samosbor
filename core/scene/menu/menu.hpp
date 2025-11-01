#pragma once 

#ifndef MENU_HPP
#define MENU_HPP

#include "scene.hpp"

#include <notcurses/notcurses.h>
#include <string>

#include "scenemanager.hpp"

class Menu : public Scene 
{
public:
    Menu(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input, SceneManager& sm);

    void Update() override;


protected:
    std::wstring MenuArt;
    int selected;

    SceneManager& sm;

    void DrawMap() override;
    void PanelDraw() override;
    void HandleInput() override;

    void DrawMenuArt();

}; 

#endif
