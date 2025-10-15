#include "../../includes/inventory/inventory.hpp"
#include "../../includes/item/item.hpp"
#include "../../includes/player/player.hpp"

#include <algorithm>

Inventory::Inventory(int capacity)
    : capacity(capacity)
{
    slots.resize(capacity);
}

Inventory::~Inventory() {}

bool Inventory::AddItem(const std::shared_ptr<Item>& item, int count)
{
    if (!item || count <= 0) return false;

    for (auto &s : slots) {
        if (s.item && s.item->GetId() == item->GetId() && s.count < s.item->GetMaxStack()) {
            int space = s.item->GetMaxStack() - s.count;
            int toAdd = std::min(space, count);
            s.count += toAdd;
            count -= toAdd;
            if (count <= 0) return true;
        }
    }

    for (auto &s : slots) {
        if (!s.item) {
            int toAdd = std::min(count, item->GetMaxStack());
            s.item = item;
            s.count = toAdd;
            count -= toAdd;
            if (count <= 0) return true;
        }
    }

    return false;
}

void Inventory::SelectNext()
{
    if (slots.empty()) return;
    selectedIndex = (selectedIndex + 1) % (int)slots.size();
}

void Inventory::SelectPrevious()
{
    if (slots.empty()) return;
    selectedIndex = (selectedIndex - 1 + (int)slots.size()) % (int)slots.size();
}

void Inventory::UseSelectedItem(Player* player)
{
    if (slots.empty()) return;
    Slot &s = slots[selectedIndex];
    if (!s.item) return;

    s.item->Use(player);

    if (s.item->IsConsumable()) {
        s.count -= 1;
        if (s.count <= 0) {
            s.item.reset();
            s.count = 0;
        }
    }
}
