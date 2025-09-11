#include "scene.hpp"
#include <iostream>
#include <vector>

Scene::Scene(const std::string& name, int height, int width) : name(name)
{

}

Scene::~Scene() = default;

std::string Scene::GetName() const {
    return name;
}

void Scene::Update(struct notcurses* nc) {

}

void Scene::Render(struct notcurses* nc) {

}