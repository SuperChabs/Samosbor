#include "scenemanager.hpp"
#include <stdexcept>
#include <iostream>

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::SetActive(const std::string& name, struct notcurses* nc) {
    if (!Has(name)) {
        throw std::runtime_error("Scene not found: " + name);
    }

    if (activeScene) {
        activeScene->Unload(nc);
    }

    activeScene = Get(name);
    activeScene->Load(nc);
}

Scene* SceneManager::GetActive() const {
    return activeScene.get();
}

void SceneManager::Update(struct notcurses* nc) {
    if (activeScene) {
        activeScene->Update(nc);
    }
}

void SceneManager::Render(struct notcurses* nc) {
    if (activeScene) {
        activeScene->Render(nc);
    }
}