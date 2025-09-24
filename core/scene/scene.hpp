#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <string>
#include <memory>

#include "inputmanager.hpp"
#include "monster.hpp"
#include "player.hpp"
#include "entity.hpp"

class Scene 
{
protected:
    struct notcurses* nc;
    struct ncplane* map;
    struct ncplane* panel;
    
    unsigned int rows, cols;
    unsigned int mapWidth, panelWidth;

    //Monster monster;
    InputManager& input;

    std::vector<std::wstring> level;

    std::vector<std::shared_ptr<Monster>> monsters;

    std::shared_ptr<Player> player;

public:
    Scene(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input);
    virtual ~Scene() = default;


    void Render();
    virtual void HandleInput(); 

protected:
    virtual void InitEntitys();
    virtual void Draw();
    virtual void PanelDraw();
    virtual void Update(ncplane* map);
};

#endif