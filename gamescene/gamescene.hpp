#pragma once 

#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "scene.hpp"

class GameScene : public Scene 
{
public:
    virtual void LoadLevel() = 0;
    virtual void SaveProgress() = 0;
    virtual void PauseGame() = 0;
    virtual void ResumeGame() = 0;

private:

    enum TileType 
    {
        FLOOR,
        WALL
    };

    std::vector<std::vector<TileType>> mapTiles;
};

#endif