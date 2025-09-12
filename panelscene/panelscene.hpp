#pragma once
#include "scene.hpp"

class PanelScene : public Scene {
public:
    PanelScene(struct notcurses* nc, int y, int x, int h, int w);

    void Update() override;
    void Render() override;
    void OnEnter() override;
    void OnExit() override;
};
