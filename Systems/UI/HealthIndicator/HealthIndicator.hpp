#pragma once
#include <SFML/Graphics.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/component.hpp>
#include <Infrastructure/entityManager.h>
#include <Common/sortByVisiableArea.hpp>
#include <Common/getPositionOnScreenByMapPos.hpp>
#include <unordered_map>

class HealthIndicator
{
private:
    static constexpr float scale = 0.25;
    static constexpr float width = 100 * scale;
    static constexpr float height = 20 * scale;
    static constexpr float thick = 5* scale;
    sf::Color BCKGfillColor = sf::Color(9.8 / 100 * 255, 23.5 / 100 * 255, 24.3 / 100 * 255);
    sf::Color fillColor = sf::Color(38.8 / 100 * 255, 78.0 / 100 * 255, 30.2 / 100 * 255);
    sf::Color fiilOutColor = sf::Color(255, 255, 255);

    bool visiableAfterDamage = false; // Optional: The indicator may be hidden after TAD(time after damage)
    //TODO: Сделать возможность отключения работы индикатора после урона
    float timeAfterDamage = 3.f;
    std::unordered_map<int, float> buff;
    std::unordered_map<int, int> healthLevel;

    // Each entity may have a HealthIndicatorShift Component
    // It means how the indicator wiil be shifted from the middle upside point of sprite
    // By the default the indicator is positioned its middle point to middle point of sprite

    // If you don't need to draw indicator for the entity you can add RejectionHealthIndicator
public:
    void findTarget(EntitiesManager& manager, std::vector<int> entityIDs) {
        for(int entityID : entityIDs) {
            if(manager.hasComponent<RejectionHealthIndicator>(entityID)) continue;

            int health = manager.getComponent<HealthComponent>(entityID)->health;
            auto it = healthLevel.find(entityID);
            if(it == healthLevel.end()) {
                healthLevel.insert(std::make_pair(entityID, health));
                continue;
            }
            
            if(healthLevel[entityID] != health) {
                healthLevel[entityID] = health;
                buff.insert(std::make_pair(entityID, 0.f));
            }
        }
    }

    void updateByTime(EntitiesManager& manager, float df) {
        std::vector<int> entityForRemove;
        for(auto& elt : buff) {
            elt.second += df;
            if(!manager.isEntityExist(elt.first) || elt.second > timeAfterDamage) {
                entityForRemove.push_back(elt.first);
                std::cout << "Удаление цели | Скрытие уровня здоровья " << elt.first << '\n';
            }
        }

        for(auto id : entityForRemove) {
            buff.erase(id);
            healthLevel.erase(id);
        }
    }

    void draw(sf::RenderTexture &window, TextureLoader &textureLoader, EntitiesManager& manager, float df) {
        std::vector<int> entityIDs = manager.with<HealthComponent>().with<SizeComponent>().with<PositionOnMapComponent>().get();
        entityIDs = sortByVisiableArea(manager, entityIDs);
        if(entityIDs.empty()) return;

        findTarget(manager, entityIDs);
        updateByTime(manager, df);

        for(auto pair : buff) {
            int entityID = pair.first;
            if(manager.hasComponent<RejectionHealthIndicator>(entityID)) continue;
            Vect2D posOnScreen = getPositionOnScreenByMapPos(manager, entityID);
            Vect2D size = manager.getComponent<SizeComponent>(entityID)->size;
            auto healthComp = manager.getComponent<HealthComponent>(entityID);
            float HP = (float)healthComp->health / healthComp->maxHealth;

            Vect2D entityUpMiddlePointOnScreen = posOnScreen + Vect2D(size.x / 2, 0);
            Vect2D healthBarPosOnScreen = entityUpMiddlePointOnScreen - Vect2D(width / 2, height);

            if(manager.hasComponent<HealthIndicatorShift>(entityID)) {
                Vect2D shift = manager.getComponent<HealthIndicatorShift>(entityID)->shift;
                healthBarPosOnScreen -= shift;
            }

            //Background
            sf::RectangleShape HealthBarBackground;
            HealthBarBackground.setPosition(healthBarPosOnScreen.x, healthBarPosOnScreen.y);
            HealthBarBackground.setSize({width, height});
            HealthBarBackground.setFillColor(BCKGfillColor);

            HealthBarBackground.setOutlineColor(fiilOutColor);
            HealthBarBackground.setOutlineThickness(thick);

            //HP
            sf::RectangleShape HealthBarFillHealth;
            HealthBarFillHealth.setPosition(healthBarPosOnScreen.x, healthBarPosOnScreen.y);
            HealthBarFillHealth.setSize({width * HP, height});
            HealthBarFillHealth.setFillColor(fillColor);

            window.draw(HealthBarBackground);
            window.draw(HealthBarFillHealth);
        }
    }
};
