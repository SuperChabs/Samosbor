#pragma once
#include "resourcemanager.hpp"
#include "scene.hpp"

class SceneManager : public ResourceManager<Scene> {
public:
    SceneManager() = default;
    ~SceneManager() override = default;

    void SetActive(const std::string& name);
    Scene* GetActive() const;

    void Update();
    void Render();

private:
    std::shared_ptr<Scene> activeScene;
};