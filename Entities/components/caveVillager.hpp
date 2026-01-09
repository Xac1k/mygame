#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"
#include "../../main.h"
#include <Entities/components/map.hpp>
#include <Entities/utils/toolTableLoader.hpp>
#include <Entities/utils/dialogComponent.hpp>

enum class VillagerState {
    IdleLeft, IdleRight,
    WalkLeft, WalkRight,
    HurtLeft, HurtRight,
    AttackIdleLeft, AttackIdleRight,
    
    DeathLeft = 451, DeathRight = 452, DeathDirect = 453, DeathBackward = 454
};
void villager(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D posOnMap) {
    int scale = 1;
    constexpr float veloWalk = TILE_SIZE * 1;
    constexpr float veloChasing = TILE_SIZE * 3;
    manager.addEntity("playMenu:Enemy:villager AIEnemy:friendly Static Deletable", true);

    PositionOnMapComponent posMap(posOnMap);
    manager.addComponent<PositionOnMapComponent>(posMap);

    SizeComponent size(64*scale, 64*scale);
    manager.addComponent<SizeComponent>(size);

    OriginComponent origin(64*scale/2, 64*scale/2);
    manager.addComponent<OriginComponent>(origin);

    HealthIndicatorShift shift(0, 0);
    manager.addComponent<HealthIndicatorShift>(shift);

    VelocityComponent velo;
    manager.addComponent<VelocityComponent>(velo);

    HealthComponent health(100, 100);
    manager.addComponent<HealthComponent>(health);

    StateComponent state((int) VillagerState::IdleLeft);
    manager.addComponent<StateComponent>(state);

    #define scaleSprite 64/32
    Vect2D sizeCollRect(18 * scale * scaleSprite, 2 * scale * scaleSprite);
    CollisionComponent rect(sizeCollRect, {(float)6 * scale * scaleSprite, (float)30 * scale * scaleSprite});
    manager.addComponent<CollisionComponent>(rect);

    WeaponComponent weaponComp(20, TILE_SIZE*2, 30, 2, 0.125f * 7);
    weaponComp.setEffect(Effects::fire, 10, 1, 1);
    manager.addComponent<WeaponComponent>(weaponComp);

//============================= DEATH =============================
    DeathComponent deathComp(0.125f * 10, "VillagerDeath", 0);
    manager.addComponent<DeathComponent>(deathComp);

    DirectionalDeathComponent dirDeathComp(Facing::Left);
    manager.addComponent<DirectionalDeathComponent>(dirDeathComp);

//============================= AI =============================
    AIAgentCompanent aiComp(HowToFindTarget::ByClassName, TILE_SIZE * 10, TILE_SIZE * 5, TILE_SIZE * 1.5, "*player*", veloWalk, veloChasing);
    manager.addComponent<AIAgentCompanent>(aiComp);

//============================= COOLDOWN =============================
    CooldownInfo colldownInfo(0.125f * 7, 2, 1.5);
    manager.addComponent(colldownInfo);

//============================= EFFECTS =============================
    EffectsComponent arrayOfEffects;
    manager.addComponent(arrayOfEffects);

    EffectsInfo effectsInfo(TILE_SIZE * 2, TILE_SIZE * 2);
    manager.addComponent(effectsInfo);

    CanFrozen canFrozen;
    manager.addComponent(canFrozen);
    CanWet canWet;
    manager.addComponent(canWet);
    CanPoisoned canPoisoned;
    manager.addComponent(canPoisoned);

//============================= ANIMATION =============================
    AnimationGridComponent animationComponent;
    animationComponent.TileSizeInGrid = {32, 32};
    loadAnimations(animationComponent.animation, 
        {
            {(int)VillagerState::IdleLeft, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 1}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 1}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 1}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 1}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 1}, 0.125f, true, {-1, 1}},
            }},
            {(int)VillagerState::IdleRight, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 1}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 1}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 1}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 1}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 1}, 0.125f, true},
            }},

            {(int)VillagerState::WalkLeft, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 2}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 2}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 2}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 2}, 0.125f, true, {-1, 1}},
            }},
            {(int)VillagerState::WalkRight, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 2}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 2}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 2}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 2}, 0.125f, true},
            }},

            {(int)VillagerState::HurtLeft, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 4}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 4}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 4}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 4}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {4, 4}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {5, 4}, 0.125f, false, {-1, 1}},
            }},
            {(int)VillagerState::HurtRight, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 4}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 4}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 4}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 4}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {4, 4}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {5, 4}, 0.125f, false},
            }},

            {(int)VillagerState::AttackIdleLeft, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 8}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 8}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 8}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 8}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {4, 8}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {5, 8}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {6, 8}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {7, 8}, 0.125f, false, {-1, 1}},
            }},
            {(int)VillagerState::AttackIdleRight, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 8}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 8}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 8}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 8}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {4, 8}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {5, 8}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {6, 8}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {7, 8}, 0.125f, false},
            }},

            {(int)VillagerState::DeathLeft, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 7}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 7}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 7}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 7}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {4, 7}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {5, 7}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {6, 7}, 0.125f, false, {-1, 1}},
                {"Store/view/CaveVillage/CaveVillage.png", {7, 7}, 0.125f, false, {-1, 1}},
            }},
            {(int)VillagerState::DeathRight, {
                {"Store/view/CaveVillage/CaveVillage.png", {0, 7}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {1, 7}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {2, 7}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {3, 7}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {4, 7}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {5, 7}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {6, 7}, 0.125f, false},
                {"Store/view/CaveVillage/CaveVillage.png", {7, 7}, 0.125f, false},
            }},
        }
    );
    manager.addComponent<AnimationGridComponent>(animationComponent);

//============================= LAYERS =============================
    OverlayesStorageComponent storage;
    manager.addComponent<OverlayesStorageComponent>(storage);
}

void addDialogToVillager(EntitiesManager& manager, std::string filename) {
    DialogTreeComponent dialogTree;
    dialogTree.loadFromTXT(filename);

    manager.addComponent(dialogTree);
}

void addDialogToVillager(EntitiesManager& manager, int ID, std::string filename) {
    DialogTreeComponent dialogTree;
    dialogTree.loadFromTXT(filename);

    manager.addComponent(dialogTree, ID);
}