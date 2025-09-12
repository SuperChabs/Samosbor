#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <notcurses/notcurses.h>

class Scene 
{
public:
    Scene(const std::string& name, struct notcurses* nc, unsigned int y, unsigned int x, unsigned int h, unsigned int w);
    virtual ~Scene();

    std::string GetName() const;

    // базові методи
    virtual void Update() = 0; 
    virtual void Render() = 0;
    virtual void OnEnter() = 0;    
    virtual void OnExit() = 0;

protected:
    std::string name;
    struct notcurses* nc;
    struct ncplane* plane;  // головний плейн для цієї сцени
    int y, x, h, w;         // позиція та розміри
};

#endif