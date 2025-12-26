#pragma once
#include <SFML/Graphics.hpp>
#include "./Interfaces/busEvent.h"
#include "./Infrastructure/sfml.h"
#include "./Entities/utils/entitiesManager.hpp"
#include "./Systems/ButtonUpdateSystem.hpp"
#include "./Systems/AnimationUpdate.hpp"
#include "./Systems/InventoryUpdateSystem.hpp"
#include <Sounds/soundManager.hpp>

#include <Entities/components/button.hpp>
#include <Entities/components/backgound.hpp>
#include <Entities/components/inventory.hpp>
#include <Entities/components/player.hpp>
#include <Entities/components/map.hpp>
#include <Entities/components/gameState.hpp>

#include <Systems/InventoryDrawSystem.hpp>
#include <Systems/InventoryContextMenuSystem.hpp>
#include <Systems/MapDrawSystem.hpp>
#include <Systems/SpawnSystem.hpp>
#include <Systems/EntitiOnMapDrawSystem.hpp>
#include <Systems/PlayerAttackSystem.hpp>
#include <Systems/HurtEntitySystem.hpp>
#include <Systems/DeathEntitySystem.hpp>
#include <Systems/DeathAnimationUpdate.hpp>
#include <Systems/ConvertorToLoot.hpp>
#include <Systems/PickUpItemsSystem.hpp>
#include <Systems/PhysicsSystem.hpp>
#include <Systems/HealthPlayerDrawSystem.hpp> 
#include <Systems/AIEnemy.hpp>
#include <Systems/StateUpdater.hpp>
#include <Systems/Cooldown.hpp>
#include <Systems/Game/Effects/EffectsSystem.hpp>

#include <Depricated/MovementPlayerSystem.hpp>

void drawPlayPage(sf::RenderWindow& window, EntitiesManager& manager, TextureLoader& textureLoader, SfmlRenderer& renderer) {
    //Systems
    HealthPlayerLevel HealthPlayerIndicator;

    window.clear(sf::Color::White);
    MapDrawSystem(window, textureLoader, manager);
    EntitiOnMapDrawSystem(window, textureLoader, manager);
    renderer.render(window, manager, textureLoader);
    InventoryDrawSystem(window, textureLoader, manager);

    HealthPlayerIndicator.draw(window, textureLoader, manager);
    window.display();
}

void PlayPage(
    sf::Clock& clock, sf::RenderWindow& window, BusEvent& busEvent,
    EntitiesManager& manager, AudioSystem& audioManager, Animator& animator,
    TextureLoader& textureLoader, SfmlRenderer& renderer
) {
    float df = clock.restart().asSeconds();
    sf::Event event;

    //Systems
    AIAgent AIAgentUpdater;
    StateUpdater StateUpdater;
    Cooldown CooldownSystem;
    EffectsSystem effectsSystem;

    while(window.pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            window.close();
        }
        busEvent.update(event);
        ButtonUpdate(manager, busEvent);
        InventoryContextMenuUpdate(manager, busEvent);
        InventoryDndUpdate(manager, busEvent);
        InitAttackPlayerSystem(manager, busEvent, audioManager);
    }
    HurtEntitySystem(manager);
    effectsSystem.update(manager, df);
    DeathEntitySystem(manager, audioManager, df);
    CreateMovementPlayerSystem(manager);
    DeathAnimationUpdateSystem(manager);
    LootDropSystem(manager, textureLoader);
    LootPickUpSystem(manager, audioManager);

    AIAgentUpdater.updateAgressiveEnemy(manager, df);
    AIAgentUpdater.defineEnemyVelocityByWandering(manager);
    AIAgentUpdater.updateEnemyPos(manager, df);

    CooldownSystem.update(manager, df);
    UpdatePhysicsSystem(manager, textureLoader, df);

    StateUpdater.updateEnemyStates(manager);
    animator.AnimationUpdate(manager, df);

    drawPlayPage(window, manager, textureLoader, renderer);
}
