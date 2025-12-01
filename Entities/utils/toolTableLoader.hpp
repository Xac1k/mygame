#pragma once
#include <filesystem>
#include "./component.hpp"

void loadLootTable(
    std::vector<LootDrop>& table, 
    std::initializer_list<LootDrop> data
) {
    for(auto& drop : data) {
        table.push_back(drop);
    }
}