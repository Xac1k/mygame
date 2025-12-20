#pragma once
#include <map>
#include <Entities/utils/component.hpp>

std::map<Items, int> limitationStack = {
    {Items::coin, 64},
    {Items::firePickaxe, 1},
    {Items::fireSpear, 1},
    {Items::fireSword, 1},
    {Items::goldenPickaxe, 1},
    {Items::goldenSpear, 1},
    {Items::goldenSword, 1},
    {Items::ironPickaxe, 1},
    {Items::ironSpear, 1},
    {Items::ironSword, 1},
    {Items::startPickaxe, 1},
    {Items::startSpear, 1},
    {Items::startSword, 1},
    {Items::skull, 10},
    {Items::bone, 64}
};