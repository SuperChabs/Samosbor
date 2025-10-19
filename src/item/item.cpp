#include <item.hpp>



Item::Item(const std::string& id, const std::string& name, wchar_t symbol,
    int maxStack, bool consumable)
    : id(id), name(name), symbol(symbol), maxStack(maxStack), consumable(consumable), onUse(nullptr) 
{
}

void Item::SetUseCallback(UseCallback callback) 
{ 
    onUse = callback; 
}

void Item::Use(Player* player) 
{
    if (onUse) 
        onUse(player);
}

const std::string& Item::GetId() const { return id; }
const std::string& Item::GetName() const { return name; }
wchar_t Item::GetSymbol() const { return symbol; }
int Item::GetMaxStack() const { return maxStack; }
bool Item::IsConsumable() const { return consumable; }