#pragma once
#include <Entities/components/map.hpp>
#include <Entities/components/player.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>

void HurtPlayerSystem(EntitiesManager& manager, float df, AudioSystem& audioManager) {
    auto playerIds = manager.withClassName("*player*");
    if(playerIds.size() == 0) return;

    auto playerVelocity = manager.getComponent<VelocityComponent>(playerIds[0]).get();
    auto playerPos = manager.getComponent<PositionOnMapComponent>(playerIds[0]).get();
    auto playerState = manager.getComponent<StateComponent>(playerIds[0]).get();
    auto playerCollisionRect = manager.getComponent<CollisionComponent>(playerIds[0]).get();
    auto playerSize = manager.getComponent<SizeComponent>(playerIds[0]).get();
    auto playerHealth = manager.getComponent<HealthComponent>(playerIds[0]).get();
    auto playerMutex = manager.getComponent<MutexComponent<ControlFlow>>(playerIds[0]).get();

    if (!playerMutex->blocked && sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
        playerMutex->blocked = true;
        playerMutex->currTime = 0;
        playerMutex->durationTime = 0.125f * 4;
        playerMutex->WhoIsUsing = ControlFlow::HurtSystem;
        audioManager.playMusic("PlayerDamage", false);

        if(playerState->state == (int)PlayerState::IdleDirect || playerState->state == (int)PlayerState::WalkDirect) {
            playerState->state = (int)PlayerState::HurtDirect;
        }
        if(playerState->state == (int)PlayerState::IdleBackward || playerState->state == (int)PlayerState::WalkBackward) {
            playerState->state = (int)PlayerState::HurtBackward;
        }
        if(playerState->state == (int)PlayerState::IdleLeft || playerState->state == (int)PlayerState::WalkLeft) {
            playerState->state = (int)PlayerState::HurtLeft;
        }
        if(playerState->state == (int)PlayerState::IdleRight || playerState->state == (int)PlayerState::WalkRight) {
            playerState->state = (int)PlayerState::HurtRight;
        }
        return;
    }

    if(playerMutex->WhoIsUsing == ControlFlow::HurtSystem) {
        playerMutex->currTime += df;
    }

    if(playerMutex->currTime > playerMutex->durationTime) {
        playerMutex->blocked = false;
        playerMutex->WhoIsUsing = ControlFlow::All;
        
        if(playerState->state == (int)PlayerState::HurtDirect) {
            playerState->state = (int)PlayerState::IdleDirect;
        }
        if(playerState->state == (int)PlayerState::HurtBackward) {
            playerState->state = (int)PlayerState::IdleBackward;
        }
        if(playerState->state == (int)PlayerState::HurtLeft) {
            playerState->state = (int)PlayerState::IdleLeft;
        }
        if(playerState->state == (int)PlayerState::HurtRight || playerState->state == (int)PlayerState::WalkRight) {
            playerState->state = (int)PlayerState::IdleRight;
        }
    }
}