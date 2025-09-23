#pragma once
#include <memory>
#include <string>
#include "resourcemanager.hpp"
#include "scene.hpp"

class SceneManager : public ResourceManager<Scene> {
private:
    std::shared_ptr<Scene> currentScene;

public:
    SceneManager() = default;
    ~SceneManager() = default;

    // Встановити активну сцену
    void SetActiveScene(const std::string& name);

    // Виклик методів активної сцени
    void Render();
    void HandleInput();
    void Update();

    bool HasActiveScene() const;
};
