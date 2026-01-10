#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Interfaces/busEvent.h>
#include <Entities/utils/component.hpp>
#include <Entities/components/inventory.hpp>
#include <Common/CollisionRect.hpp>

void InventoryContextMenuUpdate(EntitiesManager& manager, BusEvent& event) {
    auto pos = manager.with<InventoryComponent>().getComponent<PositionComponent>();
    auto size = manager.with<InventoryComponent>().getComponent<SizeComponent>();
    auto dragAndDrop = manager.with<InventoryComponent>().getComponent<DragAndDropComponent>();
    auto inventory = manager.with<InventoryComponent>().getComponent<InventoryComponent>();
    auto inventoryState = manager.with<InventoryComponent>().getComponent<StateComponent>();
    if(!pos || !size || !dragAndDrop || !inventory || !inventoryState) return;
    if(inventoryState->state == (int)InventoryState::wrapped) return;
    
    switch (event.currEvent)
    {
    case BusEvent::Event::MouseDownRight :
        if(collisionRect(event.mousePos, pos->point, size->size)) {
            float cellSizeX = size->size.x / inventory->size.x;
            float cellSizeY = size->size.y / inventory->size.y;

            Vect2D relativePoint = event.mousePos - pos->point;

            int column = std::ceil(relativePoint.x / cellSizeX);
            int row = std::ceil(relativePoint.y / cellSizeY);

            if(inventory->inventory[row-1][column-1] != Items::none) {
                inventory->selection = Vect2D(column-1, row-1);
                inventoryState->state = (int) InventoryState::selected;
                inventory->isSetContextMenu = true;
            }
        }
        break;

    case BusEvent::Event::MouseDownLeft :
        if(collisionRect(event.mousePos, pos->point, size->size) && inventory->isSetContextMenu) {
            //TODO: Сделать обработку нажатия на определённую кнопку
            event.currEvent = BusEvent::Event::None;
            inventory->isSetContextMenu = false;
            std::cout << "Нажатие на кнопку\n";
        }
        break;
    }
}