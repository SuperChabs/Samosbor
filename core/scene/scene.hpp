#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <vector>
#include <memory>
#include <notcurses/notcurses.h>

class Scene 
{
public:
    Scene(const std::string& name, struct notcurses* nc,
          unsigned int y, unsigned int x, unsigned int h, unsigned int w);
    virtual ~Scene();

    std::string GetName() const;

    // базові методи (чисто віртуальні для логіки)
    virtual void Update() = 0; 
    virtual void Render();      // тепер не чисто віртуальний
    virtual void OnEnter() = 0;    
    virtual void OnExit() = 0;

    // робота з панелями
    void AddPanel(std::shared_ptr<Scene> panel);
    void RemovePanels();

    // доступ до ncplane
    struct ncplane* GetPlane() const;

protected:
    std::string name;
    struct notcurses* nc;
    struct ncplane* plane;  // головний плейн для цієї сцени
    int y, x, h, w;         // позиція та розміри

    std::vector<std::shared_ptr<Scene>> panels;
};

#endif
