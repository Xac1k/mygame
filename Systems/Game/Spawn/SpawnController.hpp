#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/component.hpp>
#include <Systems/SpawnSystem.hpp>
#include <Systems/SpawnSystem.hpp>
#include <Systems/Game/Spawn/SpawnSystem.hpp>
#include <Systems/SpawnBarrelSystem.hpp>
#include <Entities/components/dialogPoint.hpp>
#include <Entities/components/FinalPoint.hpp>

class SpawnController
{
private:
    int prevLevel = 0;

    SpawnBarrelSystem SpawnBarrel;
    SpawnSystem SpawnOre;
public:
    void update(EntitiesManager& manager, TextureLoader& textureLoader) {
        auto gameStateIDs = manager.with<GameStateComponent>().get();
        if(gameStateIDs.size() == 0) return;
        auto gameStateComp = manager.getComponent<GameStateComponent>(gameStateIDs[0]); 
        if(prevLevel == gameStateComp->level) return;
        prevLevel = gameStateComp->level;

        auto mapIDs = manager.with<MapComponent>().get();
        if(mapIDs.size() == 0) return;
        auto mapComponent = manager.getComponent<MapComponent>(mapIDs[0]);

        auto playerIDs = manager.withClassName("*player*");
        if(playerIDs.size() == 0) return;
        auto player = manager.getComponent<PositionOnMapComponent>(playerIDs[0]);

        manager.removeEntityByClass("*Deletable*");
        if(gameStateComp->level == 1) {
            mapComponent->map.loadFromFile("Map/Levels/level1.txt");
            player->point = mapComponent->map.getSpawnPoint();

            dialogPoint(manager, player->point, "Store/data/Dialogs/StarterDialogWithLio.json");
            villager(manager, textureLoader, player->point + Vect2D(42*TILE_SIZE, 0));
            addDialogToVillager(manager, "Store/data/Dialogs/StarterDialogWithMikonid.json");
        }
        else if(gameStateComp->level > 2 && gameStateComp->level == 3) {
            mapComponent->map.loadFromFile("Map/Levels/Taverna.txt");
            player->point = mapComponent->map.getSpawnPoint();

            villager(manager, textureLoader, player->point + Vect2D(0, 15*TILE_SIZE));
            addDialogToVillager(manager, "Store/data/Dialogs/StarterDialogWithMikonid.json");
        }
        else if(gameStateComp->level == 11) {
            gameStateComp->screen = GameScreen::end;
        }
        else {
            mapType type = mapType::Rect;
            if(randFloat() > 0.5) type = mapType::Circle;
            mapComponent->map.generateMap(type);
            player->point = mapComponent->map.getSpawnPoint();

            finalPoint(manager, mapComponent->map.getFinalPoint(player->point), "Store/data/Dialogs/ExitPointDialog.json");
            
            SpawnOre.update(manager, textureLoader);
            SpawnBarrel.update(manager, textureLoader);
            
        }

        //SpawnSystemUpdate(manager, textureLoader);
    }
};
