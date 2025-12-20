#pragma once
#include <Entities/components/map.hpp>
#include <Entities/components/player.hpp>
#include <Entities/components/goblin.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>
#include <Sounds/soundManager.hpp>
#include <Interfaces/busEvent.h>
#include <Common/getAngle.hpp>
#include <Map/Creating/Common/dist.hpp>
#include <Map/Creating/Common/randomRange.hpp>
#include <Entities/components/items.hpp>
#include <Common/randFloat.hpp>
#include <Common/getMiddleAngle.hpp>

class StateUpdater
{
private:
    void updateStateSkeletons(EntitiesManager& manager) {
        auto enemyIDs = manager.withClassName("*skeleton*");
        if(enemyIDs.size() == 0) return;

        for (int enemyID : enemyIDs) {
            auto facingComp = manager.getComponent<DirectionalDeathComponent>(enemyID).get();
            auto veloComp = manager.getComponent<VelocityComponent>(enemyID).get();
            auto stateComp = manager.getComponent<StateComponent>(enemyID).get();

            if(manager.hasComponent<AttackComponent>(enemyID)) {
                auto attackComp = manager.getComponent<AttackComponent>(enemyID).get();
                auto angelArea = attackComp->attackArea;
                auto angle = angleMidpoint(angelArea.x, angelArea.y);
                if(90 < angle && angle < 270) {
                    stateComp->state = (int)SkeletonState::AttackIdleLeft;
                }
                else {
                    stateComp->state = (int)SkeletonState::AttackIdleRight;
                }
                continue;
            }

            if(manager.hasComponent<HurtComponent>(enemyID)) {
                auto angle = manager.getComponent<HurtComponent>(enemyID).get()->angle;
                if(90 < angle && angle < 270) {
                    stateComp->state = (int)SkeletonState::HurtLeft;
                }
                else {
                    stateComp->state = (int)SkeletonState::HurtRight;
                }
                manager.removeComponent<HurtComponent>(enemyID);
                continue;
            }

            if(manager.hasComponent<StuneCompanent>(enemyID)) continue;

            if(0 < veloComp->dir.x) {
                stateComp->state = (int)SkeletonState::WalkRight;
                facingComp->facing = Facing::Rigth;
            }
            else if(0 > veloComp->dir.x) {
                stateComp->state = (int)SkeletonState::WalkLeft;
                facingComp->facing = Facing::Left;
            }
            else if(0 != veloComp->dir.y) {
                if(stateComp->state == (int)PlayerState::IdleLeft) {
                    stateComp->state = (int)SkeletonState::WalkLeft;
                    facingComp->facing = Facing::Left;
                }
                if(stateComp->state == (int)PlayerState::IdleRight) {
                    stateComp->state = (int)SkeletonState::WalkRight;
                    facingComp->facing = Facing::Rigth;
                }
            }
            else {
                if(stateComp->state == (int)SkeletonState::WalkRight) stateComp->state = (int)SkeletonState::IdleRight;
                if(stateComp->state == (int)SkeletonState::WalkLeft) stateComp->state = (int)SkeletonState::IdleLeft;
                if(stateComp->state == (int)SkeletonState::AttackIdleRight) stateComp->state = (int)SkeletonState::IdleRight;
                if(stateComp->state == (int)SkeletonState::AttackIdleLeft) stateComp->state = (int)SkeletonState::IdleLeft;
                if(stateComp->state == (int)SkeletonState::HurtRight) stateComp->state = (int)SkeletonState::IdleRight;
                if(stateComp->state == (int)SkeletonState::HurtLeft) stateComp->state = (int)SkeletonState::IdleLeft;
            }
        }   
    }

    void updateStatePlayer(EntitiesManager& manager) {
        auto playerIds = manager.withClassName("*player*");
        if(playerIds.size() == 0) return;

        auto facingComp = manager.getComponent<DirectionalDeathComponent>(playerIds[0]).get();
        auto playerVelocity = manager.getComponent<VelocityComponent>(playerIds[0]).get();
        auto playerState = manager.getComponent<StateComponent>(playerIds[0]).get();

        if(manager.hasComponent<HurtComponent>(playerIds[0])) {
            auto angle = manager.getComponent<HurtComponent>(playerIds[0]).get()->angle;
            if(90 < angle && angle < 270) {
                playerState->state = (int)PlayerState::HurtLeft;
            }
            else {
                playerState->state = (int)PlayerState::HurtRight;
            }
            manager.removeComponent<HurtComponent>(playerIds[0]);
            return;
        }

        if(manager.hasComponent<StuneCompanent>(playerIds[0])) return;

        if(0 < playerVelocity->dir.x) {
            playerState->state = (int)PlayerState::WalkRight;
            facingComp->facing = Facing::Rigth;
        }
        else if(0 > playerVelocity->dir.x) {
            playerState->state = (int)PlayerState::WalkLeft;
            facingComp->facing = Facing::Left;
        }
        else if(0 < playerVelocity->dir.y) {
            playerState->state = (int)PlayerState::WalkDirect;
            facingComp->facing = Facing::Direct;
        }
        else if(0 > playerVelocity->dir.y) {
            playerState->state = (int)PlayerState::WalkBackward;
            facingComp->facing = Facing::Backward;
        }
        else {
            if(playerState->state == (int)PlayerState::WalkRight) playerState->state = (int)PlayerState::IdleRight;
            if(playerState->state == (int)PlayerState::WalkLeft) playerState->state = (int)PlayerState::IdleLeft;
            if(playerState->state == (int)PlayerState::WalkDirect) playerState->state = (int)PlayerState::IdleDirect;
            if(playerState->state == (int)PlayerState::WalkBackward) playerState->state = (int)PlayerState::IdleBackward;

            if(playerState->state == (int)PlayerState::HurtRight) playerState->state = (int)PlayerState::IdleRight;
            if(playerState->state == (int)PlayerState::HurtLeft) playerState->state = (int)PlayerState::IdleLeft;
            if(playerState->state == (int)PlayerState::HurtDirect) playerState->state = (int)PlayerState::IdleDirect;
            if(playerState->state == (int)PlayerState::HurtBackward) playerState->state = (int)PlayerState::IdleBackward;

            if(playerState->state == (int)PlayerState::AttackIdleRight) playerState->state = (int)PlayerState::IdleRight;
            if(playerState->state == (int)PlayerState::AttackIdleLeft) playerState->state = (int)PlayerState::IdleLeft;
            if(playerState->state == (int)PlayerState::AttackIdleDirect) playerState->state = (int)PlayerState::IdleDirect;
            if(playerState->state == (int)PlayerState::AttackIdleBackward) playerState->state = (int)PlayerState::IdleBackward;
        }
    }
public:
    void updateEnemyStates(EntitiesManager& manager) {
        updateStateSkeletons(manager);
        updateStatePlayer(manager);
    }
};
