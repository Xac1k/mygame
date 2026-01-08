#pragma once
#include <Common/const.hpp>
#include <Entities/utils/dialogComponent.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>

void dialogPoint(EntitiesManager& manager, Vect2D posOnMap, std::string localPath) {
    manager.addEntity("playMenu:DialogPoint Deletable Static", true);

    PositionOnMapComponent posMap(posOnMap);
    manager.addComponent<PositionOnMapComponent>(posMap);

    DialogTreeComponent dialogTree;
    dialogTree.loadFromTXT(localPath);
    manager.addComponent(dialogTree);

    DialogStartOnOver startOnOver;
    manager.addComponent(startOnOver);
}