#pragma once

#include "scene.hpp"
#include "inputmanager.hpp"

#include <ncurses.h>

class Level1 : public Scene 
{
public:
    Level1(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input);

    void Update(ncplane* map) override;

private:
    void InitEntitys();
    void DrawMap() override;
    void PanelDraw() override;

};