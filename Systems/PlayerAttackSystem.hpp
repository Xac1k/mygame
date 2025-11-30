#pragma once
#include <Entities/components/map.hpp>
#include <Entities/components/player.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>
#include <Sounds/soundManager.hpp>
#include <Interfaces/busEvent.h>
#include <Common/getAngle.hpp>

void addAttackComp(EntitiesManager& manager, int ID, float angle) {
    auto playerPos = manager.getComponent<PositionOnMapComponent>(ID).get()->point;
    auto playerWeapon = manager.getComponent<WeaponComponent>(ID).get();
    Vect2D attackArea(angle - playerWeapon->deviation, angle + playerWeapon->deviation);
    if(attackArea.x > 360) attackArea.x -= 360;
    if(attackArea.y > 360) attackArea.y -= 360;
    AttackComponent attack(playerWeapon->damage, playerPos, attackArea, playerWeapon->atackLength);
    manager.addComponent<AttackComponent>(attack, ID);
}

void setMutex(MutexComponent<ControlFlow> *playerMutex) {
    playerMutex->blocked = true;
    playerMutex->currTime = 0;
    playerMutex->durationTime = 0.125f * 8;
    playerMutex->WhoIsUsing = ControlFlow::AttackSystem;
}

void InitAttackPlayerSystem(EntitiesManager& manager, BusEvent& event, AudioSystem& audioManager) {
    auto playerIds = manager.withClassName("*player*");
    if(playerIds.size() == 0) return;
    if(event.currEvent != BusEvent::Event::MouseDownLeft) return;

    auto playerState = manager.getComponent<StateComponent>(playerIds[0]).get();
    auto playerMutex = manager.getComponent<MutexComponent<ControlFlow>>(playerIds[0]).get();

    if (!playerMutex->blocked ) {
        setMutex(playerMutex);
        audioManager.playMusic("PlayerAttack", false);
        //TODO: Сделать анимации для атаки во время ходьбы
        float angle = getAngle(event.mousePos - Vect2D(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
        addAttackComp(manager, playerIds[0], angle);
        
        if(
            playerState->state == (int)PlayerState::IdleBackward ||
            playerState->state == (int)PlayerState::IdleDirect ||
            playerState->state == (int)PlayerState::IdleLeft ||
            playerState->state == (int)PlayerState::IdleRight
        ) {
            if(angle > 0 && angle < 45 || angle > 345 && angle < 360) playerState->state = (int)PlayerState::AttackIdleRight;
            if(angle > 45 && angle < 135) playerState->state = (int)PlayerState::AttackIdleDirect;
            if(angle > 135 && angle < 225) playerState->state = (int)PlayerState::AttackIdleLeft;
            if(angle > 225 && angle < 345) playerState->state = (int)PlayerState::AttackIdleBackward;
        }
        return;
    }
}

void UpdateAttackPlayerSystem(EntitiesManager& manager, float df) {
    auto playerIds = manager.withClassName("*player*");
    if(playerIds.size() == 0) return;

    auto playerVelocity = manager.getComponent<VelocityComponent>(playerIds[0]).get();
    auto playerPos = manager.getComponent<PositionOnMapComponent>(playerIds[0]).get();
    auto playerState = manager.getComponent<StateComponent>(playerIds[0]).get();
    auto playerHealth = manager.getComponent<HealthComponent>(playerIds[0]).get();
    auto playerMutex = manager.getComponent<MutexComponent<ControlFlow>>(playerIds[0]).get();

    if(playerMutex->WhoIsUsing == ControlFlow::AttackSystem) {
        playerMutex->currTime += df;
    }

    if(playerMutex->currTime > playerMutex->durationTime) {
        playerMutex->blocked = false;
        playerMutex->WhoIsUsing = ControlFlow::All;
        
        if(playerState->state == (int)PlayerState::AttackIdleDirect) {
            playerState->state = (int)PlayerState::IdleDirect;
        }
        if(playerState->state == (int)PlayerState::AttackIdleBackward) {
            playerState->state = (int)PlayerState::IdleBackward;
        }
        if(playerState->state == (int)PlayerState::AttackIdleLeft) {
            playerState->state = (int)PlayerState::IdleLeft;
        }
        if(playerState->state == (int)PlayerState::AttackIdleRight) {
            playerState->state = (int)PlayerState::IdleRight;
        }
    }
}