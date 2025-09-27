#pragma once
#include "scene.hpp"
#include "inputmanager.hpp"
#include "scenemanager.hpp"
#include <vector>
#include <string>

class MenuScene : public Scene {
public:
    MenuScene(notcurses* nc, ncplane* stdn, unsigned int rows, unsigned int cols,
              InputManager& input, SceneManager& sm, bool& running);
    ~MenuScene() override;

    void DrawMap() override;
    void PanelDraw() override;
    void SetupInput() override;

private:
    SceneManager& scenemanager;
    bool& running;
    size_t selected;
    std::vector<std::string> options;


    ncplane* panel = nullptr;
};
