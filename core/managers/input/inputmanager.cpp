#include <notcurses/notcurses.h>

#include "inputmanager.hpp"

void InputManager::Bind(int key, std::function<void()> action){
    bindings[key] = action;
}

void InputManager::Update(struct notcurses* nc){
    struct ncinput ni;
    int ret = notcurses_get(nc, nullptr, &ni);
    if(ret > 0){
        auto it = bindings.find(ni.id);
        if(it != bindings.end()){
            it->second(); // викликаємо прив’язану функцію
        }
    }
}

void InputManager::UpdateOnce(struct notcurses* nc){
    struct ncinput ni;
    int ret = notcurses_get(nc, nullptr, &ni);
    if(ret <= 0) return; // нічого не натиснуто

    // враховуємо тільки натискання
    if(ni.evtype != NCTYPE_PRESS) return;

    auto it = bindings.find(ni.id);
    if(it != bindings.end()){
        it->second(); // викликаємо дію один раз
    }
}