#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Interfaces/busEvent.h>
#include <Entities/utils/component.hpp>
#include <Entities/components/inventory.hpp>
#include <Common/CollisionRect.hpp>
#include <Entities/components/inventory.hpp>

void InventoryDndUpdate(EntitiesManager& manager, BusEvent& event) {
    auto pos = manager.with<InventoryComponent>().getComponent<PositionComponent>();
    auto size = manager.with<InventoryComponent>().getComponent<SizeComponent>();
    auto dragAndDrop = manager.with<InventoryComponent>().getComponent<DragAndDropComponent>();
    auto inventory = manager.with<InventoryComponent>().getComponent<InventoryComponent>();
    auto inventoryState = manager.with<InventoryComponent>().getComponent<StateComponent>();
    if(!pos || !size || !dragAndDrop || !inventory || !inventoryState) return;

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
                    dragAndDrop->isStarted = true;
                    inventory->selection = Vect2D(column-1, row-1);
                    inventoryState->state = (int) InventoryState::selected;
                    inventory->isSelected = true;
                }
            }
            break;

        case BusEvent::Event::MouseMove :
            if(dragAndDrop->isStarted) {
                dragAndDrop->pos = event.mousePos;
                dragAndDrop->isMoving = true;
            }
            break;

        case BusEvent::Event::MouseUp :
            if(dragAndDrop->isStarted && dragAndDrop->isMoving)
            {
                float cellSizeX = size->size.x / inventory->size.x;
                float cellSizeY = size->size.y / inventory->size.y;

                Vect2D relativePoint = event.mousePos - pos->point;

                int column = std::ceil(relativePoint.x / cellSizeX) - 1;
                int row = std::ceil(relativePoint.y / cellSizeY) - 1;

                if(row < 0 || column < 0 || row > inventory->size.y - 1 || column > inventory->size.x - 1) {
                    inventoryState->state = (int) InventoryState::idle;
                    dragAndDrop->isMoving = false;
                    dragAndDrop->isStarted = false;
                    break;
                }

                auto moveableItem = inventory->inventory[inventory->selection.y][inventory->selection.x];
                auto moveableItemCount = inventory->countItems[inventory->selection.y][inventory->selection.x];
                auto targetItem = inventory->inventory[row][column];
                auto targetItemCount = inventory->countItems[row][column];

                inventory->inventory[inventory->selection.y][inventory->selection.x] = targetItem;
                inventory->countItems[inventory->selection.y][inventory->selection.x] = targetItemCount;
                inventory->inventory[row][column] = moveableItem;
                inventory->countItems[row][column] = moveableItemCount;
                inventory->selection = Vect2D(column, row);
            }
        
            dragAndDrop->isMoving = false;
            dragAndDrop->isStarted = false;
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