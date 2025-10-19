#pragma once

#include "scene.hpp"
#include "inputmanager.hpp"

#include <ncurses.h>
#include <string>
#include "panelmanager.hpp"

class GameScene : public Scene 
{
private:
    std::string hud;
    PanelManager panelManager;

public:
    GameScene(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input);

    void Update() override;

    ncplane* GetMap() override;
    ncplane* GetPanel() override;

private:
    void InitEntitys();
    void DrawMap() override;
    void PanelDraw() override;
    void GenerateAutoDungeon(int roomCount = 12);
 
    std::vector<std::pair<int,int>> roomCenters;

};