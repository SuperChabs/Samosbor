#include "scene.hpp"

Scene::Scene(const std::string& name, struct notcurses* nc,
             unsigned int y, unsigned int x, unsigned int h, unsigned int w)
    : name(name), nc(nc), y(y), x(x), h(h), w(w)
{
    struct ncplane_options opts = {};
    opts.y = y;
    opts.x = x;
    opts.rows = h;
    opts.cols = w;
    opts.userptr = this;
    plane = ncplane_create(notcurses_stdplane(nc), &opts);
}

Scene::~Scene() {
    if (plane) {
        ncplane_destroy(plane);
    }
}

std::string Scene::GetName() const {
    return name;
}

void Scene::Render() {
    // кожна дочірня сцена сама малює себе
    for (auto& p : panels) {
        p->Render();
    }
}

void Scene::AddPanel(std::shared_ptr<Scene> panel) {
    panels.push_back(panel);
}

void Scene::RemovePanels() {
    panels.clear();
}

struct ncplane* Scene::GetPlane() const {
    return plane;
}
