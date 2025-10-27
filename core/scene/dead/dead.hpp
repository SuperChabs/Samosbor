#pragma once 

#ifndef DEAD_HPP
#define DEAD_HPP

#include "scene.hpp"

#include <notcurses/notcurses.h>
#include <string>

#include "scenemanager.hpp"

class Dead : public Scene 
{
public:
    Dead(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input, SceneManager& sm);

    void Update() override;


private:
    std::wstring MenuArt;
    int selected;

    SceneManager& sm;

    void DrawMap() override;
    void PanelDraw() override;
    void HandleInput() override;

    void DrawMenuArt();

}; 

#endif
