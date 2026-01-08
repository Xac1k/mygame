#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"

void gameState(EntitiesManager& manager) {
    manager.addEntity();
    
    GameStateComponent gameState;
    manager.addComponent<GameStateComponent>(gameState);

    ChunkLoaderComponent chunkForProcessing(2);
    manager.addComponent<ChunkLoaderComponent>(chunkForProcessing);

    
}



