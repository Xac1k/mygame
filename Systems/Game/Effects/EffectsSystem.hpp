#pragma once
#include <Entities/components/map.hpp>
#include <Entities/components/player.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>
#include <Sounds/soundManager.hpp>
#include <Interfaces/busEvent.h>
#include <Common/getAngle.hpp>
#include <Map/Creating/Common/dist.hpp>
#include <Entities/utils/animationLoader.hpp>

class EffectsSystem
{
private:
    void addEffectOverlayAnimation(EntitiesManager& manager, int enemyID, Effects effect) {
        if(!manager.hasComponent<OverlayesStorageComponent>(enemyID)) return;
        auto anims = manager.getComponent<OverlayesStorageComponent>(enemyID).get();
        AnimationGrid overlayAnim;
        switch (effect)
        {
        case Effects::fire:
            loadOverlayAnimations(overlayAnim, {{
                {"Store/view/Effects/664.png", {0, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {1, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {2, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {3, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {4, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {5, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {6, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {7, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {8, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {9, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {10, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {11, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {12, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {13, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {14, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {15, 0}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {16, 0}, 0.125f, false, {1, 1}, 50, true},
            }});
            break;
        case Effects::frozen:
            loadOverlayAnimations(overlayAnim, {{
                {"Store/view/Effects/664.png", {0, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {1, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {2, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {3, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {4, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {5, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {6, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {7, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {8, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {9, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {10, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {11, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {12, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {13, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {14, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {15, 2}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {16, 2}, 0.125f, false, {1, 1}, 50, true},
            }});
            break;
        case Effects::wet:
            loadOverlayAnimations(overlayAnim, {{
                {"Store/view/Effects/664.png", {0, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {1, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {2, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {3, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {4, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {5, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {6, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {7, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {8, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {9, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {10, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {11, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {12, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {13, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {14, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {15, 8}, 0.0625f, false, {1, -1}, 50},
                {"Store/view/Effects/664.png", {16, 8}, 0.0625f, false, {1, -1}, 50, true},
            }});
            break;
        case Effects::poisoned:
            loadOverlayAnimations(overlayAnim, {{
                {"Store/view/Effects/664.png", {0, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {1, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {2, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {3, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {4, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {5, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {6, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {7, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {8, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {9, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {10, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {11, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {12, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {13, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {14, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {15, 3}, 0.125f, false, {1, 1}, 50},
                {"Store/view/Effects/664.png", {16, 3}, 0.125f, false, {1, 1}, 50, true},
            }});
            break;
        
        default:
            break;
        }

        OverlayAnimationComponent overlay;
        overlay.overlayAnim = overlayAnim;
        overlay.priority = 1;
        overlay.TileSizeInGrid = Vect2D(64, 64);
        overlay.size = Vect2D(128, 128);

        anims->overlayes.push_back(overlay);
    }

    void addMixEffectOverlayAnimation(EntitiesManager& manager, int enemyID, bool frozen, bool fire, bool wet) {
        if(!manager.hasComponent<OverlayesStorageComponent>(enemyID)) return;
        auto anims = manager.getComponent<OverlayesStorageComponent>(enemyID).get();
        AnimationGrid overlayAnim;
        if(frozen && fire) {
            loadOverlayAnimations(overlayAnim, {{
                {"Store/view/Effects/673.png", {0, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {1, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {2, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {3, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {4, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {5, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {6, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {7, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {8, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {9, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {10, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {11, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {12, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {13, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {14, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {15, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {16, 2}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {17, 2}, 0.0625f, false, {1, 1}, 100, true},
            }});
        }
        if(wet && fire) {
            loadOverlayAnimations(overlayAnim, {{
                {"Store/view/Effects/673.png", {0, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {1, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {2, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {3, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {4, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {5, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {6, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {7, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {8, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {9, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {10, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {11, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {12, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {13, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {14, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {15, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {16, 5}, 0.0625f, false, {1, 1}},
                {"Store/view/Effects/673.png", {17, 5}, 0.0625f, false, {1, 1}, 100, true},
            }});
        }
        OverlayAnimationComponent overlay;
        overlay.overlayAnim = overlayAnim;
        overlay.priority = 1;
        overlay.TileSizeInGrid = Vect2D(64, 64);
        overlay.size = Vect2D(128, 128);

        anims->overlayes.push_back(overlay);
    }

    void updateCollisionEffects(EntitiesManager& manager, std::vector<int> enemyIDs) {
        for(auto enemyID : enemyIDs) {
            auto effects = manager.getComponent<EffectsComponent>(enemyID).get();
            
            bool haveFire = effects->find(Effects::fire) != effects->effects.end();
            bool haveWet = effects->find(Effects::wet) != effects->effects.end();
            bool haveFrozen = effects->find(Effects::frozen) != effects->effects.end();
            bool havePoisoned = effects->find(Effects::poisoned) != effects->effects.end();

            if(haveFire && haveWet) {
                effects->removeEffect(Effects::fire);
                effects->removeEffect(Effects::wet);
                addMixEffectOverlayAnimation(manager, enemyID, haveFrozen, haveFire, haveWet);
            }

            if(haveFire && haveFrozen) {
                auto itFrozen = effects->find(Effects::frozen);
                auto itFire = effects->find(Effects::fire);

                auto healthComp =  manager.getComponent<HealthComponent>(enemyID).get();
                healthComp->health -= (itFire.base()->damage + itFrozen.base()->damage) ;

                effects->removeEffect(Effects::fire);
                effects->removeEffect(Effects::frozen);
                addMixEffectOverlayAnimation(manager, enemyID, haveFrozen, haveFire, haveWet);
            }
        }
    }

    void updateEffectsByTime(EntitiesManager& manager, std::vector<int> enemyIDs, float df) {
        for(auto enemyID : enemyIDs) {
            auto effects = manager.getComponent<EffectsComponent>(enemyID).get();
            for(auto& effect : effects->effects) {
                effect.currentTime += df;

                if(effect.currentTime > effect.duration)
                    effects->removeEffect(effect);

                if(effect.currentTime > effect.period * (effect.count + 1)) {
                    effect.count++;

                    // check haven't health
                    if(!manager.hasComponent<HealthComponent>(enemyID)) {
                        manager.removeComponent<EffectsComponent>(enemyID);
                        continue;
                    }

                    auto healthComp =  manager.getComponent<HealthComponent>(enemyID).get();
                    healthComp->health -= effect.damage;
                    addEffectOverlayAnimation(manager, enemyID, effect.effect);
                }
            }
        }
    }

    void updateForOtherEntitiesByTargetDist(EntitiesManager& manager, std::vector<int> enemyIDs) {
        for(auto enemyID : enemyIDs) {
            auto effects = manager.getComponent<EffectsComponent>(enemyID).get();
        }
    }

public:
    void update(EntitiesManager& manager, float df) {
        auto enemyIDs = manager.with<EffectsComponent>().get();
        //updateForOtherEntitiesByTargetDist(manager, enemyIDs);
        updateCollisionEffects(manager, enemyIDs);
        updateEffectsByTime(manager, enemyIDs, df);
    }
};