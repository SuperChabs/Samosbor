#include "scenemanager.hpp"
#include <stdexcept>

void SceneManager::SetActive(const std::string& name) {
    auto it = resources.find(name);
    if (it == resources.end()) {
        throw std::runtime_error("Сцена з ім'ям '" + name + "' не знайдена");
    }

    if (activeScene) {
        activeScene->OnExit();
    }

    activeScene = it->second;
    activeScene->OnEnter();
}

Scene* SceneManager::GetActive() const {
    return activeScene.get();
}

void SceneManager::Update() {
    if (activeScene) {
        activeScene->Update();
    }
}

void SceneManager::Render() {
    if (activeScene) {
        activeScene->Render();
    }
}
