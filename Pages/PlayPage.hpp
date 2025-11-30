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
#include <Systems/MovementPlayerSystem.hpp>
#include <Systems/HurtPlayerSystem.hpp>
#include <Systems/EntitiOnMapDrawSystem.hpp>
#include <Systems/PlayerAttackSystem.hpp>
#include <Systems/HurtEntitySystem.hpp>
#include <Systems/DeathEntitySystem.hpp>
#include <Systems/DeathAnimationUpdate.hpp>

void PlayPage(
    sf::Clock& clock, sf::RenderWindow& window, BusEvent& busEvent,
    EntitiesManager& manager, AudioSystem& audioManager, Animator& animator,
    TextureLoader& textureLoader, SfmlRenderer& renderer
) {
    float df = clock.restart().asSeconds();
    sf::Event event;

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
    HurtPlayerSystem(manager, df, audioManager);
    UpdateAttackPlayerSystem(manager, df);
    HurtEntitySystem(manager);
    DeathEntitySystem(manager, audioManager);
    MovementPlayerSystem(manager, df);
    DeathAnimationUpdateSystem(manager);
    animator.AnimationUpdate(manager, df);

    window.clear(sf::Color::White);
    MapDrawSystem(window, textureLoader, manager);
    EntitiOnMapDrawSystem(window, textureLoader, manager);
    renderer.render(window, manager, textureLoader);
    InventoryDrawSystem(window, textureLoader, manager);
    window.display();
}