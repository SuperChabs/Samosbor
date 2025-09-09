#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <vector>
#include <notcurses/notcurses.h>

class Scene {
public:
    explicit Scene(const std::string& name);
    virtual ~Scene();

    std::string GetName() const;

    // Методи, які можна перевизначати
    virtual void Load(struct notcurses* nc);   
    virtual void Update(struct notcurses* nc); 
    virtual void Render(struct notcurses* nc); 
    virtual void Unload(struct notcurses* nc); 

private:
    std::string name;

    enum TileType 
    {
        FLOOR,
        WALL
    };

    std::vector<std::vector<TileType>> mapTiles;
};

#endif