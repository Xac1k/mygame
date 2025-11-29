#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Interfaces/busEvent.h>
#include <Entities/utils/component.hpp>
#include <Entities/components/inventory.hpp>
#include <Common/CollisionRect.hpp>

void InventoryDrawSystem(
    sf::RenderWindow &window, TextureLoader &textureLoader, 
    EntitiesManager& manager
) {
    auto entityIDs = manager.with<InventoryComponent>().get();
    if(entityIDs.size() < 1) return;

    int entityID = entityIDs[0];

    auto inventory = manager.getComponent<InventoryComponent>(entityID).get();
    auto inventoryState = manager.getComponent<StateComponent>(entityID).get();

    auto pos = manager.getComponent<PositionComponent>(entityID).get();
    auto size = manager.getComponent<SizeComponent>(entityID).get();

    auto dndComponent = manager.getComponent<DragAndDropComponent>(entityID).get();

    Vect2D cellSize = Vect2D(size->size.x / inventory->size.x,  size->size.y / inventory->size.y);
    if(inventoryState->state == (int)InventoryState::wrapped) {
        cellSize = Vect2D(size->size.x / inventory->size.x,  size->size.y);
    }

    if(
        inventory->isSelected && 
        (
            inventoryState->state == (int)InventoryState::wrapped ||
            inventoryState->state == (int)InventoryState::selected
        )
    ) {
        Vect2D select = inventory->selection;
        Vect2D selectionPos = select * cellSize + pos->point;

        sf::Sprite sprite = textureLoader.getSprite("Store/view/Inventory/SelectionCell.png", selectionPos, cellSize);
        window.draw(sprite);
    };

    //Рисовка не выделенных объектов
    int x = 0; int y = 0;
    for(std::vector<Items> items : inventory->inventory) {
        for(Items item : items) {
            if(item != Items::none) {
                if(
                    inventoryState->state == (int)InventoryState::selected && 
                    x == inventory->selection.x &&
                    y == inventory->selection.y
                ) {
                    x++;
                    continue;
                };

                Vect2D posItem = Vect2D(x, y) * cellSize + pos->point + cellSize * 0.2;
                Vect2D sizeItem = cellSize - cellSize * 0.4;

                sf::Sprite sprite = textureLoader.getSprite(
                    "Store/view/Items/item" + std::to_string((int)item) + ".png",
                    posItem,
                    sizeItem
                );
                window.draw(sprite);
            }
            x++;
        }
        x = 0;
        y++;
        if(inventoryState->state == (int)InventoryState::wrapped) break;
    }

    //Рисовка выделенного объекта на верхнем слое
    if(inventoryState->state == (int)InventoryState::selected){
        Items item = inventory->inventory[inventory->selection.y][inventory->selection.x];
        Vect2D posItem = inventory->selection * cellSize + pos->point + cellSize * 0.2;
        Vect2D sizeItem = cellSize - cellSize * 0.4;
        if(dndComponent->isMoving == true) {
            posItem = dndComponent->pos;
        }

        sf::Sprite sprite = textureLoader.getSprite(
            "Store/view/Items/item" + std::to_string((int)item) + ".png",
            posItem,
            sizeItem
        );
        window.draw(sprite);
    }

    //Рисовка контекстного меню
    if(inventoryState->state == (int)InventoryState::selected && inventory->isSetContextMenu) {
        Items item = inventory->inventory[inventory->selection.y][inventory->selection.x];
        Vect2D posContextUseBtn = inventory->selection * cellSize + pos->point + Vect2D(cellSize.x, 0);
        Vect2D sizeContextUseBtn = Vect2D(96, 32);
        sf::Sprite spriteUseBtn = textureLoader.getSprite(
            "Store/view/Inventory/UseButton.png",
            posContextUseBtn,
            sizeContextUseBtn
        );
        window.draw(spriteUseBtn);

        Vect2D posContextDelBtn = inventory->selection * cellSize + pos->point + Vect2D(cellSize.x, 32);
        Vect2D sizeContextDelBtn = Vect2D(96, 32);
        sf::Sprite spriteDelBtn = textureLoader.getSprite(
            "Store/view/Inventory/DeleteButton.png",
            posContextDelBtn,
            sizeContextDelBtn
        );
        window.draw(spriteDelBtn);

        Vect2D posContextMenu = inventory->selection * cellSize + pos->point + Vect2D(cellSize.x, 0);
        Vect2D sizeContextMenu = Vect2D(96, 64);
        sf::Sprite sprite = textureLoader.getSprite(
            "Store/view/Inventory/BorderChoiseMenu2.png",
            posContextMenu,
            sizeContextMenu
        );
        window.draw(sprite);
    }
}