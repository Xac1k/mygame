#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "./Interfaces/busEvent.h"
#include "./Infrastructure/sfml.h"
#include <Entities/utils/entitiesManager.hpp>
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
#include <Systems/UI/HealthIndicator/HealthIndicator.hpp>
#include <Systems/UI/DialogWindow/DialogWindow.hpp>
#include <Systems/Game/Spawn/SpawnController.hpp>

#include <Depricated/MovementPlayerSystem.hpp>

SpawnController LevelController;
    
HealthIndicator EntityHealthIndicator;
DialogWindow DialogManager;
extern sf::Shader DrunkShader;
extern sf::RenderTexture renderTexture;
extern sf::Vector2f rescaleCoeff;

AIAgent AIAgentUpdater;

sf::Clock clockForDrunk;
#define BIAS_IN_PIXEL 200.0f
#define PIXEL_COEF 999.0f

void drawPlayPage(sf::RenderWindow& window, EntitiesManager& manager, TextureLoader& textureLoader, SfmlRenderer& renderer, float df) {
    //Systems
    HealthPlayerLevel HealthPlayerIndicator;
    renderTexture.clear(sf::Color::White);

    MapDrawSystem(renderTexture, textureLoader, manager);
    EntitiOnMapDrawSystem(renderTexture, textureLoader, manager);
    EntityHealthIndicator.draw(renderTexture, textureLoader, manager, df);
    HealthPlayerIndicator.draw(renderTexture, textureLoader, manager);
    

    renderTexture.display();
    sf::Sprite sprite(renderTexture.getTexture());

    float time = clockForDrunk.getElapsedTime().asMilliseconds();
    DrunkShader.setUniform("time", time);
    DrunkShader.setUniform("bias", BIAS_IN_PIXEL);
    DrunkShader.setUniform("resolution", sf::Vector2f((WINDOW_WIDTH + 80) * rescaleCoeff.x, (WINDOW_HEIGHT) * rescaleCoeff.y));
    DrunkShader.setUniform("pixelizeCoeff", PIXEL_COEF);

    window.clear();
    window.draw(sprite, &DrunkShader);
    renderer.render(window, manager, textureLoader);
    InventoryDrawSystem(window, textureLoader, manager);
    DialogManager.UI(window, manager, textureLoader);
    window.display();
}

void PlayPage(
    sf::Clock& clock, sf::RenderWindow& window, BusEvent& busEvent,
    EntitiesManager& manager, AudioSystem& audioManager, Animator& animator,
    TextureLoader& textureLoader, SfmlRenderer& renderer
) {
    LevelController.update(manager, textureLoader);
    float df = clock.restart().asSeconds();
    sf::Event event;

    //Systems
    StateUpdater StateUpdater;
    Cooldown CooldownSystem;
    EffectsSystem effectsSystem;
    
    while(window.pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            window.close();
        }
        if(event.type == sf::Event::Resized) {
            sf::Vector2u winSize = window.getSize();
            rescaleCoeff.x = (float)winSize.x / WINDOW_WIDTH;
            rescaleCoeff.y = (float)winSize.y / WINDOW_HEIGHT;
            std::cout << "Разрешение изменено коофициент пропорции: " << rescaleCoeff.x << 'x' << rescaleCoeff.y << std::endl;
        }
        busEvent.update(event, rescaleCoeff);
        ButtonUpdate(manager, busEvent);
        if(DialogManager.canPlay()) {
            InventoryContextMenuUpdate(manager, busEvent);
            InventoryDndUpdate(manager, busEvent);
            InitAttackPlayerSystem(manager, busEvent, audioManager);
        }
    }

    if(DialogManager.canPlay()) {
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
    }

    StateUpdater.updateEnemyStates(manager);
    animator.AnimationUpdate(manager, df);

    DialogManager.Controller(manager);
    drawPlayPage(window, manager, textureLoader, renderer, df);
}
