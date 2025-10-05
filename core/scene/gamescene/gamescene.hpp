#pragma once

#include "scene.hpp"
#include "inputmanager.hpp"

#include <ncurses.h>
#include <string>

class GameScene : public Scene 
{
private:
    std::string hud;
    //std::string debugText;

public:
    GameScene(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input);

    void Update(ncplane* map) override;

private:
    void InitEntitys();
    void DrawMap() override;
    void PanelDraw() override;
    void GenerateAutoDungeon(int roomCount = 12);
    //void ChangeLevel();
    // centers of rooms generated in GenerateComplexMap (x,y) pairs
    std::vector<std::pair<int,int>> roomCenters;

};