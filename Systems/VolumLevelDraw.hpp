#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/component.hpp>

void VolumeLevelDrawSystem(
    sf::RenderWindow &window, TextureLoader &textureLoader, 
    EntitiesManager& manager
) {
    auto entityIDs = manager.with<GameStateComponent>().get();
    if(entityIDs.size() < 1) return;

    int entityID = entityIDs[0];
    auto gameState = manager.getComponent<GameStateComponent>(entityID).get();

    Vect2D pos(250, 190);
    Vect2D size(200, 25);

    auto level = textureLoader.getSprite("Store/view/Lable/Level.png", pos, size);
    window.draw(level);
    auto fullCell = textureLoader.getSprite("Store/view/Lable/LevelHigh.png", pos, {size.x/10, size.y}); 

    for(int i = 0; i < gameState->volume; i++) {
        fullCell.setPosition(pos.x + size.x/10 * i, pos.y);
        window.draw(fullCell);
    }
    
}