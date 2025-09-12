#pragma once
#include "scene.hpp"
#include "player.hpp"
#include "monster.hpp"

class GameScene : public Scene {
public:
    GameScene(struct notcurses* nc, int h, int w);

    void Update() override;
    void Render() override;
    void OnEnter() override;
    void OnExit() override;

private:
    Player player;
    Monster monster;
};
