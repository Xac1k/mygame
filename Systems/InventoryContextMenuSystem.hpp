#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Interfaces/busEvent.h>
#include <Entities/utils/component.hpp>
#include <Entities/components/inventory.hpp>
#include <Common/CollisionRect.hpp>

void InventoryContextMenuUpdate(EntitiesManager& manager, BusEvent& event) {
    auto inventoryIDs = manager.with<InventoryComponent>().get(); 
    if(inventoryIDs.size() < 1) return;
    
    auto pos = manager.getComponent<PositionComponent>(inventoryIDs[0]).get();
    auto size = manager.getComponent<SizeComponent>(inventoryIDs[0]).get();
    auto dragAndDrop = manager.getComponent<DragAndDropComponent>(inventoryIDs[0]);

    auto inventory = manager.getComponent<InventoryComponent>(inventoryIDs[0]).get();
    auto inventoryState = manager.getComponent<StateComponent>(inventoryIDs[0]).get();
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
        }
        break;
    }
}