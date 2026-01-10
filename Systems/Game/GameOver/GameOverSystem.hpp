#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <Interfaces/busEvent.h>
#include <Infrastructure/sfml.h>
#include <Entities/utils/entitiesManager.hpp>
#include <Systems/ButtonUpdateSystem.hpp>
#include <Systems/AnimationUpdate.hpp>
#include <Systems/InventoryUpdateSystem.hpp>
#include <Sounds/soundManager.hpp>

class GameOverSystem
{
private:
public:
    void update(EntitiesManager& manager) {
        auto playerDeath = manager.with<DeathComponent>().withClassName("*player*").getComponent<DeathComponent>();
        if(!playerDeath) return;

        auto gameState = manager.with<GameStateComponent>().getComponent<GameStateComponent>();
        if(!gameState) return;
        if(playerDeath->isDead) gameState->screen = GameScreen::gameover;
    }
};

