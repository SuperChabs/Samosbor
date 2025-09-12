#include "panelscene.hpp"

PanelScene::PanelScene(struct notcurses* nc, int y, int x, int h, int w)
    : Scene("Panel", nc, y, x, h, w)
{}

void PanelScene::Update() {}

void PanelScene::Render() {
    ncplane_erase(plane);

    ncplane_set_fg_rgb8(plane, 255, 255, 255);
    ncplane_putstr_yx(plane, 1, 1, "== Панель ==");
    ncplane_putstr_yx(plane, 3, 1, "HP: 100");
    ncplane_putstr_yx(plane, 5, 1, "Інвентар:");
    ncplane_putstr_yx(plane, 6, 3, "- Меч");
}

void PanelScene::OnEnter() {}
void PanelScene::OnExit() {}
