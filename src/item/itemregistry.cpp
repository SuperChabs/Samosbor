#include "itemregistry.hpp"
#include "player.hpp"
#include <iostream>
#include <cstdlib>

std::unordered_map<std::string, std::shared_ptr<Item>> ItemRegistry::items;

void ItemRegistry::InitializeItems() 
{
    auto healthPotion = std::make_shared<Item>(
        "health_potion",
        "Зілля здоров'я",
        L'\u2665',
        16,
        true
    );
    healthPotion->SetUseCallback([](Player* player) 
    {
        player->ModifyHealth(20);
        //std::cout << "Використано зілля! +20 HP" << std::endl;
        //panelManager.SetHUDLine(18, "", 255, 255, 255);
    });
    RegisterItem(healthPotion);

    auto bigHealthPotion = std::make_shared<Item>(
        "big_health_potion",
        "Велике зілля",
        L'\u2665',
        8,
        true
    );
    bigHealthPotion->SetUseCallback([](Player* player) 
    {
        player->ModifyHealth(50);
        //std::cout << "Використано велике зілля! +50 HP" << std::endl;
    });
    RegisterItem(bigHealthPotion);

    auto poisonPotion = std::make_shared<Item>(
        "poison_potion",
        "Отруйне зілля",
        L'\u2620',
        16,
        true
    );
    poisonPotion->SetUseCallback([](Player* player) {
        player->ModifyHealth(-10);
        //std::cout << "Використано отруту! -10 HP" << std::endl;
    });
    RegisterItem(poisonPotion);

    auto exitBook = std::make_shared<Item>(
        "exit_book",
        "Книга виходу",
        L'\u26a0',
        1,
        false
    );
    exitBook->SetUseCallback([](Player* player) {
        //std::cout << "Ви використали книгу виходу! Гра завершується..." << std::endl;
        //std::exit(0);
    });
    RegisterItem(exitBook);

    auto key = std::make_shared<Item>(
        "key",
        "Ключ",
        L'\u26bf',
        16,
        false
    );
    key->SetUseCallback([](Player* player) {
        //std::cout << "Використано ключ! (поки нічого не відбувається)" << std::endl;
    });
    RegisterItem(key);

    auto trash = std::make_shared<Item>(
        "trash",
        "Смердючий мусор",
        L'\u203b',
        16,
        false
    );
    trash->SetUseCallback([](Player* player) 
    {
        //std::cout << "Ви нюхнули мусор... Фу!" << std::endl;
    });
    RegisterItem(trash);

}
