#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Interfaces/busEvent.h>
#include <Entities/utils/component.hpp>
#include <Entities/components/inventory.hpp>
#include <Common/CollisionRect.hpp>
#include <Entities/components/inventory.hpp>

void InventoryDndUpdate(EntitiesManager& manager, BusEvent& event) {
    auto inventoryIDs = manager.with<InventoryComponent>().get(); 
    if(inventoryIDs.size() < 1) return;
    
    auto pos = manager.getComponent<PositionComponent>(inventoryIDs[0]).get();
    auto size = manager.getComponent<SizeComponent>(inventoryIDs[0]).get();
    auto dragAndDrop = manager.getComponent<DragAndDropComponent>(inventoryIDs[0]);

    auto inventory = manager.getComponent<InventoryComponent>(inventoryIDs[0]).get();
    auto inventoryState = manager.getComponent<StateComponent>(inventoryIDs[0]).get();

    if (event.currEvent == BusEvent::Event::I) {
        if(inventoryState->state != (int)InventoryState::wrapped) {
            inventoryState->state = (int)InventoryState::wrapped;
            pos->point = wrapedPos;
            size->size = wrapedSize;   
        }
        else {
            inventoryState->state = (int)InventoryState::idle;
            pos->point = unwrapedPos;
            size->size = unwrapedSize;
        }
    }
    

    if(inventoryState->state != (int)InventoryState::wrapped) {
        switch (event.currEvent)
        {
        case BusEvent::Event::MouseDownLeft :
            if(collisionRect(event.mousePos, pos->point, size->size)) {
                float cellSizeX = size->size.x / inventory->size.x;
                float cellSizeY = size->size.y / inventory->size.y;

                Vect2D relativePoint = event.mousePos - pos->point;

                int column = std::ceil(relativePoint.x / cellSizeX);
                int row = std::ceil(relativePoint.y / cellSizeY);

                if(inventory->inventory[row-1][column-1] != Items::none) {
                    dragAndDrop.get()->isStarted = true;
                    std::cout << "Dnd started" << std::endl;
                    inventory->selection = Vect2D(column-1, row-1);
                    inventoryState->state = (int) InventoryState::selected;
                    inventory->isSelected = true;
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
    else {
        switch (event.currEvent)
        {
        case BusEvent::Event::Num1:
            if(inventory->isSelected && inventory->selection.x == 0){
                inventory->isSelected = false;
            } else {
                inventory->isSelected = true;
                inventory->selection = Vect2D(0, 0);
            }
            break;
        
        case BusEvent::Event::Num2:
            if(inventory->isSelected && inventory->selection.x == 1){
                inventory->isSelected = false;
            } else {
                inventory->isSelected = true;
                inventory->selection = Vect2D(1, 0);
            }
            break;

        case BusEvent::Event::Num3:
            if(inventory->isSelected && inventory->selection.x == 2){
                inventory->isSelected = false;
            } else {
                inventory->isSelected = true;
                inventory->selection = Vect2D(2, 0);
            }
            break;

        case BusEvent::Event::Num4:
            if(inventory->isSelected && inventory->selection.x == 3){
                inventory->isSelected = false;
            } else {
                inventory->isSelected = true;
                inventory->selection = Vect2D(3, 0);
            }
            break;

        case BusEvent::Event::Num5:
            if(inventory->isSelected && inventory->selection.x == 4){
                inventory->isSelected = false;
            } else {
                inventory->isSelected = true;
                inventory->selection = Vect2D(4, 0);
            }
            break;
        
        default:
            break;
        }
        
    }
}