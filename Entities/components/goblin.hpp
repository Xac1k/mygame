#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"
#include "../../main.h"
#include <Entities/components/map.hpp>
#include <Entities/utils/toolTableLoader.hpp>


enum class SkeletonType {none, Fire, Wet, Poisoned};
enum class SkeletonState { 
    IdleLeft, IdleRight,
    WalkLeft, WalkRight,
    HurtLeft, HurtRight,
    AttackIdleLeft, AttackIdleRight,
    
    DeathLeft = 451, DeathRight = 452, DeathDirect = 453, DeathBackward = 454
};
void skeleton(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D posOnMap, SkeletonType type) {
    int scale = 2;
    constexpr float veloWalk = TILE_SIZE * 1;
    constexpr float veloChasing = TILE_SIZE * 3;
    manager.addEntity("playMenu:Enemy:skeleton AIEnemy:agressive Dynamic Deletable", true);

    PositionOnMapComponent posMap(posOnMap);
    manager.addComponent<PositionOnMapComponent>(posMap);

    SizeComponent size(96*scale, 64*scale);
    manager.addComponent<SizeComponent>(size);

    OriginComponent origin(96*scale/2, 64*scale/2);
    manager.addComponent<OriginComponent>(origin);

    HealthIndicatorShift shift(0, -32);
    manager.addComponent<HealthIndicatorShift>(shift);

    VelocityComponent velo;
    manager.addComponent<VelocityComponent>(velo);

    HealthComponent health(100, 100);
    manager.addComponent<HealthComponent>(health);

    StateComponent state((int) SkeletonState::IdleLeft);
    manager.addComponent<StateComponent>(state);

    Vect2D sizeCollRect(13*scale, 10*scale);
    CollisionComponent rect(sizeCollRect, {(float)42*scale, (float)30*scale});
    manager.addComponent<CollisionComponent>(rect);

    WeaponComponent weaponComp(20, TILE_SIZE*2, 30, 2, 0.125f * 7);
    switch (type)
    {
    case SkeletonType::Fire:
        weaponComp.setEffect(Effects::fire, 10, 1, 1);
        break;
    case SkeletonType::Wet:
        weaponComp.setEffect(Effects::wet, 10, 1.55f, 0);
        break;
    case SkeletonType::Poisoned:
        weaponComp.setEffect(Effects::poisoned, 10, 2.f, 4);
        break;
    default:
        break;
    }
    
    manager.addComponent<WeaponComponent>(weaponComp);

//============================= DEATH =============================
    DeathComponent deathComp(0.125f * 10, "SkeletonDeath", 0);
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
    EffectComponent effect;
    switch (type)
    {
    case SkeletonType::Fire:
        effect.effect = Effects::fire;
        effect.duration = 3600.f;
        effect.period = 2;
        arrayOfEffects.effects.push_back(effect);
        break;
    case SkeletonType::Wet:
        effect.effect = Effects::wet;
        effect.duration = 3600.f;
        effect.period = 1.55f;
        arrayOfEffects.effects.push_back(effect);
        break;
    case SkeletonType::Poisoned:
        effect.effect = Effects::poisoned;
        effect.duration = 3600.f;
        effect.period = 2;
        arrayOfEffects.effects.push_back(effect);
        break;
    default:
        break;
    }
    manager.addComponent(arrayOfEffects);

    EffectsInfo effectsInfo(TILE_SIZE * 2, TILE_SIZE * 2);
    manager.addComponent(effectsInfo);

//============================= LOOT =============================
    LootTableComponent lootTable;
    loadLootTable(lootTable.drops, {
        {Items::coin, 1, 3, 10, 30},
        {Items::skull, 0.5, 1, 1, 30},
        {Items::bone, 0.7, 1, 4, 30},
    });
    manager.addComponent<LootTableComponent>(lootTable);

//============================= ANIMATION =============================
    AnimationGridComponent animationComponent;
    animationComponent.TileSizeInGrid = {96, 64};
    loadAnimations(animationComponent.animation, 
        {
            {(int)SkeletonState::IdleLeft, {
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {0, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {1, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {2, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {3, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {4, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {5, 0}, 0.125f, true, {-1, 1}},
            }},
            {(int)SkeletonState::IdleRight, {
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {0, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {1, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {2, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {3, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {4, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_idle_strip6.png", {5, 0}, 0.125f, true},
            }},

            {(int)SkeletonState::WalkLeft, {
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {0, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {1, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {2, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {3, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {4, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {5, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {6, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {7, 0}, 0.125f, true, {-1, 1}},
            }},
            {(int)SkeletonState::WalkRight, {
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {0, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {1, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {2, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {3, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {4, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {5, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {6, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_walk_strip8.png", {7, 0}, 0.125f, true},
            }},

            {(int)SkeletonState::HurtLeft, {
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {0, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {1, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {2, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {3, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {4, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {5, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {6, 0}, 0.125f, false, {-1, 1}},
            }},
            {(int)SkeletonState::HurtRight, {
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {0, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {1, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {2, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {3, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {4, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {5, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_hurt_strip7.png", {6, 0}, 0.125f, false},
            }},

            {(int)SkeletonState::AttackIdleLeft, {
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {0, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {1, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {2, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {3, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {4, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {5, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {6, 0}, 0.125f, false, {-1, 1}},
            }},
            {(int)SkeletonState::AttackIdleRight, {
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {0, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {1, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {2, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {3, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {4, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {5, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_attack_strip7.png", {6, 0}, 0.125f, false},
            }},

            {(int)SkeletonState::DeathLeft, {
                {"Store/view/Skeleton/skeleton_death_strip10.png", {0, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {1, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {2, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {3, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {4, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {5, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {6, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {7, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {8, 0}, 0.125f, false, {-1, 1}},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {9, 0}, 0.125f, false, {-1, 1}},
            }},
            {(int)SkeletonState::DeathRight, {
                {"Store/view/Skeleton/skeleton_death_strip10.png", {0, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {1, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {2, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {3, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {4, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {5, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {6, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {7, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {8, 0}, 0.125f, false},
                {"Store/view/Skeleton/skeleton_death_strip10.png", {9, 0}, 0.125f, false},
            }},
        }
    );
    manager.addComponent<AnimationGridComponent>(animationComponent);

//============================= LAYERS =============================
    OverlayesStorageComponent storage;
    manager.addComponent<OverlayesStorageComponent>(storage);
}