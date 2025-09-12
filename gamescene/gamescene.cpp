#include "gamescene.hpp"

GameScene::GameScene(struct notcurses* nc, int h, int w)
    : Scene("GameScene", nc, 0, 0, h, w),
      player(w/2, h/2),
      monster(w/4, h/4)
{}

void GameScene::Update() {
    // тут логіка монстра / гравця
}

void GameScene::Render() {
    ncplane_erase(plane);

    // фон
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            ncplane_set_fg_rgb8(plane, 60, 60, 60);
            ncplane_putstr_yx(plane, yy, xx, ".");
        }
    }

    player.Render(plane);
    monster.Render(plane);
}

void GameScene::OnEnter() {}
void GameScene::OnExit() {}
