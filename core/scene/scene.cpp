#include "scene.hpp"
#include <iostream>

Scene::Scene(const std::string& name) : name(name) {}
Scene::~Scene() = default;

std::string Scene::GetName() const {
    return name;
}

void Scene::Load(struct notcurses* nc) {
    std::cout << "Loading scene: " << name << std::endl;
}

void Scene::Update(struct notcurses* nc) {
    std::cout << "Updating scene: " << name << std::endl;
}

void Scene::Render(struct notcurses* nc) {
    std::cout << "Rendering scene: " << name << std::endl;
}

void Scene::Unload(struct notcurses* nc) {
    std::cout << "Unloading scene: " << name << std::endl;
}