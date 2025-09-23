#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <string>

#include "inputmanager.hpp"
#include "monster.hpp"
#include "player.hpp"

class Scene 
{
private:
    struct notcurses* nc;
    struct ncplane* map;
    struct ncplane* panel;
    unsigned int rows, cols;
    unsigned int mapWidth, panelWidth;

    Player player;
    Monster monster;
    InputManager& input;

    std::vector<std::wstring> level;

public:
    Scene(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input);

    void Render();
    
private:
    virtual void HandleInput(); 
    virtual void Draw();
    virtual void PanelDraw();
    virtual void Update(ncplane* map);
    

};

#endif