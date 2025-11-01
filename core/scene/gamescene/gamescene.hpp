#pragma once

#include "scene.hpp"
#include "inputmanager.hpp"
#include "panelmanager.hpp"
#include "scenemanager.hpp"

#include <ncurses.h>
#include <string>


class GameScene : public Scene 
{
private:
    std::string hud;
    PanelManager& panelManager;
    SceneManager& sm;

public:
    GameScene(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, 
            InputManager& input, PanelManager& panelManager, SceneManager& sm);

    void Show() override;

    void Update() override;

    ncplane* GetMap() override;
    ncplane* GetPanel() override;

private:
    void InitEntitys();

    void DrawMap() override;
    void PanelDraw() override;

    void AdditinalInput() override;

    void GenerateAutoDungeon(int roomCount = 12);
 
    std::vector<std::pair<int,int>> roomCenters;

};