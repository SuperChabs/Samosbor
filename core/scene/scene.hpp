#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <vector>
#include <notcurses/notcurses.h>

class Scene 
{
public:

    explicit Scene(const std::string& name, int height, int width);
    virtual ~Scene();

    std::string GetName() const;
  
    virtual void Update(struct notcurses* nc); 
    virtual void Render(struct notcurses* nc); 
    virtual void OnEnter() = 0;    
    virtual void OnExit() =  0;

private:
    std::string name;
};

#endif