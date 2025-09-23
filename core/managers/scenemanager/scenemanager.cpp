#include "scenemanager.hpp"
#include <stdexcept>

void SceneManager::SetActiveScene(const std::string& name) {
    if (!Has(name)) {
        throw std::runtime_error("Scene '" + name + "' not found!");
    }
    currentScene = Get(name);
}

void SceneManager::Render() {
    if (currentScene) {
        currentScene->Render();
    }
}

void SceneManager::HandleInput() {
    if (currentScene) {
        currentScene->HandleInput();
    }
}

void SceneManager::Update() {
    if (currentScene) {
        // Якщо захочеш винести логіку окремо від Render
        // currentScene->Update();
    }
}

bool SceneManager::HasActiveScene() const {
    return (bool)currentScene;
}
