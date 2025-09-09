#pragma once
#include "resourcemanager.hpp"
#include "scene.hpp"

class SceneManager : public ResourceManager<Scene> {
public:
    SceneManager();
    ~SceneManager() override;

    void SetActive(const std::string& name, struct notcurses* nc);
    Scene* GetActive() const;

    void Update(struct notcurses* nc);
    void Render(struct notcurses* nc);

private:
    std::shared_ptr<Scene> activeScene;
};