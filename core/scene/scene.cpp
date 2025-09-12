#include "scene.hpp"

Scene::Scene(const std::string& name, struct notcurses* nc, unsigned int y, unsigned int x, unsigned int h, unsigned int w)
    : name(name), nc(nc), y(y), x(x), h(h), w(w)
{
    struct ncplane_options opts = { .y = (int)y, .x = (int)x, .rows = h, .cols = w };
    plane = ncplane_create(notcurses_stddim_yx(nc, NULL, NULL), &opts);
}

Scene::~Scene()
{
    if (plane) {
        ncplane_destroy(plane);
    }
}

std::string Scene::GetName() const {
    return name;
}