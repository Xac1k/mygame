#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/component.hpp>
#include <Systems/SpawnSystem.hpp>
#include <Systems/SpawnSystem.hpp>
#include <Systems/Game/Spawn/SpawnSystem.hpp>
#include <Entities/components/dialogPoint.hpp>
#include <Entities/components/FinalPoint.hpp>
#include <Sounds/soundManager.hpp>
#include <Systems/Game/Spawn/SpawnSystemBlank.hpp>
#include <Entities/components/goblin.hpp>

class SpawnSkeletonSystem : public SpawnSystemBlankIntoRoom {
    void spawnEntityFunc(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D spawnPos) override {
        SkeletonType type;
        float chance = randFloat();
        if(chance < 0.25) 
            type = SkeletonType::Fire;
        else if(chance < 0.5)
            type = SkeletonType::Wet;
        else if(chance < 0.75)
            type = SkeletonType::Poisoned;
        else 
            type = SkeletonType::none;

        skeleton(manager, textureLoader, spawnPos, type);
    }
};

class SpawnBarrelSystem : public SpawnSystemBlankIntoRoom {
    void spawnEntityFunc(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D spawnPos) override {
        barrel(manager, textureLoader, spawnPos);
    }
};

class SpawnController
{
private:
    int prevLevel = 0;

    SpawnBarrelSystem SpawnBarrel;
    SpawnSystem SpawnOre;
    SpawnSkeletonSystem SpawnSkeleton; 
public:
    SpawnController() {
        SpawnBarrel.entityInRoom = {3, 20};
    };

    void update(EntitiesManager& manager, TextureLoader& textureLoader, AudioSystem& audioManager) {
        SpawnSkeleton.ENTER_TOLERANCE = 6 * TILE_SIZE;
        SpawnSkeleton.update(manager, textureLoader);
        SpawnBarrel.update(manager, textureLoader);

        auto gameStateIDs = manager.with<GameStateComponent>().get();
        if(gameStateIDs.size() == 0) return;
        auto gameStateComp = manager.getComponent<GameStateComponent>(gameStateIDs[0]); 
        if(prevLevel == gameStateComp->level) return;
        prevLevel = gameStateComp->level;

        SpawnSkeleton.reset();
        SpawnBarrel.reset();

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
        else if(gameStateComp->level == 11) {
            gameStateComp->screen = GameScreen::end;
            audioManager.playMusic("firework", false);
        }
        else if(gameStateComp->level > 2 && gameStateComp->level % 2 == 1) {
            mapComponent->map.loadFromFile("Map/Levels/Taverna.txt");
            audioManager.pauseMusic("playMusic");
            audioManager.playMusic("TavernAmbitient");
            player->point = mapComponent->map.getSpawnPoint();

            villager(manager, textureLoader, player->point + Vect2D(-1*TILE_SIZE, -21*TILE_SIZE));
            addDialogToVillager(manager, "Store/data/Dialogs/StoreDialog.json");

            finalPoint(manager, player->point - Vect2D(1.5*TILE_SIZE, 1*TILE_SIZE), "Store/data/Dialogs/ExitPointDialog.json");
             
            villager(manager, textureLoader, player->point + Vect2D(-6*TILE_SIZE, -12*TILE_SIZE));
            addDialogToVillager(manager, "Store/data/Dialogs/Quest" + std::to_string(gameStateComp->level) + ".json");
        }
        else {
            audioManager.pauseMusic("TavernAmbitient");
            audioManager.playMusic("playMusic");
            mapType type = mapType::Rect;
            if(randFloat() > 0.5) type = mapType::Circle;
            mapComponent->map.generateMap(type);
            player->point = mapComponent->map.getSpawnPoint();

            finalPoint(manager, mapComponent->map.getFinalPoint(player->point), "Store/data/Dialogs/ExitPointDialog.json");
            
            SpawnOre.update(manager, textureLoader);
            SpawnBarrel.update(manager, textureLoader);
        }
    }
};
