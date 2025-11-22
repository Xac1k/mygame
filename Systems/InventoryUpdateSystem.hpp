#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Interfaces/busEvent.h>
#include <Entities/utils/component.hpp>
#include <Entities/components/inventory.hpp>
#include <Common/CollisionRect.hpp>

void InventoryDndUpdate(EntitiesManager& manager, BusEvent& event) {
    auto inventoryIDs = manager.with<InventoryComponent>().get(); 

    auto pos = manager.getComponent<PositionComponent>(inventoryIDs[0]).get();
    auto size = manager.getComponent<SizeComponent>(inventoryIDs[0]).get();
    auto dragAndDrop = manager.getComponent<DragAndDropComponent>(inventoryIDs[0]);

    auto inventory = manager.getComponent<InventoryComponent>(inventoryIDs[0]).get();
    auto inventoryState = manager.getComponent<StateComponent>(inventoryIDs[0]).get();
    switch (event.currEvent)
    {
    case BusEvent::Event::MouseDownLeft :
        if(collisionRect(event.mousePos, pos->point, size->size)) {
            dragAndDrop.get()->isStarted = true;
            std::cout << "Dnd started" << std::endl;

            float cellSizeX = size->size.x / inventory->size.x;
            float cellSizeY = size->size.y / inventory->size.y;

            Vect2D relativePoint = event.mousePos - pos->point;

            int column = std::ceil(relativePoint.x / cellSizeX);
            int row = std::ceil(relativePoint.y / cellSizeY);

            if(inventory->inventory[row-1][column-1] != Items::none) {
                inventory->selection = Vect2D(column-1, row-1);
                inventoryState->state = (int) InventoryState::selected;
            }
        }
        break;

    case BusEvent::Event::MouseMove :
        if(dragAndDrop.get()->isStarted) {
            dragAndDrop.get()->pos = event.mousePos;
            dragAndDrop.get()->isMoving = true;
            std::cout << "Dnd moving" << std::endl;
        }
        break;

    case BusEvent::Event::MouseUp :
        if(dragAndDrop.get()->isStarted && dragAndDrop.get()->isMoving)
        {
            std::cout << "Drag and Drop end" << std::endl;
            float cellSizeX = size->size.x / inventory->size.x;
            float cellSizeY = size->size.y / inventory->size.y;

            Vect2D relativePoint = event.mousePos - pos->point;

            int column = std::ceil(relativePoint.x / cellSizeX) - 1;
            int row = std::ceil(relativePoint.y / cellSizeY) - 1;

            if(row < 0 || column < 0 || row > inventory->size.y - 1 || column > inventory->size.x - 1) {
                inventoryState->state = (int) InventoryState::idle;
                dragAndDrop.get()->isMoving = false;
                dragAndDrop.get()->isStarted = false;
                break;
            }

            auto moveableItem = inventory->inventory[inventory->selection.y][inventory->selection.x];
            auto targetItem = inventory->inventory[row][column];

            Items tempItem = targetItem;

            inventory->inventory[inventory->selection.y][inventory->selection.x] = tempItem;
            inventory->inventory[row][column] = moveableItem;
            inventory->selection = Vect2D(column, row);
        }
    
        dragAndDrop.get()->isMoving = false;
        dragAndDrop.get()->isStarted = false;
        break;
    
    default:
        break;
    }
}