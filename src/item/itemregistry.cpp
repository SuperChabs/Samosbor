#include "itemregistry.hpp"
#include "player.hpp"
#include "panelmanager.hpp"

#include <cstdlib>
#include <thread>
#include <chrono>


std::unordered_map<std::string, std::shared_ptr<Item>> ItemRegistry::items;

void ItemRegistry::RegisterItem(std::shared_ptr<Item> item) 
{
    if (item) 
        items[item->GetId()] = item;
}

std::shared_ptr<Item> ItemRegistry::GetItem(const std::string& id) 
{
    auto it = items.find(id);
    if (it != items.end()) 
        return it->second;

    return nullptr;
}

const std::unordered_map<std::string, std::shared_ptr<Item>>& ItemRegistry::GetAllItems() 
{
    return items;
}

void ItemRegistry::InitializeItems(PanelManager& panelManager) 
{
    auto healthPotion = std::make_shared<Item>(
        "health_potion",
        "Зілля здоров'я",
        L'\u2665',
        16,
        true
    );
    healthPotion->SetUseCallback([pm = &panelManager](Player* player) 
    {
        player->ModifyHealth(20);
        pm->Clear();
        pm->SetHUDLine(26, "Використано зілля здоров'я! +20 хп", 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        pm->Clear();
    });
    RegisterItem(healthPotion);

    auto bigHealthPotion = std::make_shared<Item>(
        "big_health_potion",
        "Велике зілля",
        L'\u2665',
        8,
        true
    );
    bigHealthPotion->SetUseCallback([pm = &panelManager](Player* player) 
    {
        player->ModifyHealth(50);
        pm->Clear();
        pm->SetHUDLine(26, "Використано ключ! (поки нічого не відбувається)", 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        pm->Clear();
    });
    RegisterItem(bigHealthPotion);

    auto poisonPotion = std::make_shared<Item>(
        "poison_potion",
        "Отруйне зілля",
        L'\u2620',
        16,
        true
    );
    poisonPotion->SetUseCallback([pm = &panelManager](Player* player) {
        player->ModifyHealth(-10);
        pm->Clear();
        pm->SetHUDLine(26, "Використано отруйне зілл'я! -10 хп", 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        pm->Clear();
    });
    RegisterItem(poisonPotion);

    auto exitBook = std::make_shared<Item>(
        "exit_book",
        "Книга виходу",
        L'\u26a0',
        1,
        false
    );
    exitBook->SetUseCallback([pm = &panelManager](Player* player) {
        pm->Clear();
        pm->SetHUDLine(26, "Використано книгу виходу! Виходиму з гри", 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        Settings::Instance().SetRunning(false);
    });
    RegisterItem(exitBook);

    auto key = std::make_shared<Item>(
        "key",
        "Ключ",
        L'\u26bf',
        16,
        false
    );
    key->SetUseCallback([pm = &panelManager](Player* player) {
        pm->Clear();
        pm->SetHUDLine(26, "Використано ключ! (поки нічого не відбувається)", 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        pm->Clear();
    });
    RegisterItem(key);

    auto trash = std::make_shared<Item>(
        "trash",
        "Смердючий мусор",
        L'\u203b',
        16,
        false
    );
    trash->SetUseCallback([pm = &panelManager](Player* player) 
    {
        pm->Clear();
        pm->SetHUDLine(26, "Фу, лайно!", 255, 255, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        pm->Clear();
    });
    RegisterItem(trash);

}
