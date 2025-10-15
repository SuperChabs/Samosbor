#include "scenemanager.hpp"
#include <stdexcept>

void SceneManager::SetActiveScene(const std::string& name) 
{
    if (!Has(name)) 
    {
        throw std::runtime_error("Scene '" + name + "' not found!");
    }
    // hide previous scene if any
    if (currentScene) currentScene->Hide();

    currentScene = Get(name);

    // show the new current scene
    if (currentScene) currentScene->Show();
}

void SceneManager::Render() 
{
    if (currentScene) 
    {
        currentScene->Render();
    }
}

void SceneManager::HandleInput() 
{
    if (currentScene) 
    {
        currentScene->HandleInput();
    }
}

void SceneManager::Update() 
{
    if (currentScene) 
    {
        currentScene->Update();
    }
}

bool SceneManager::HasActiveScene() const 
{
    return (bool)currentScene;
}