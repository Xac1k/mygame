#pragma once
#include <SFML/Graphics.hpp>
#include <Infrastructure/entityManager.h>
#include <Entities/utils/entitiesManager.hpp>
#include <Infrastructure/sfml.h>
#include <queue>
#include <main.h>

struct Enemy {
    float x, y;
    int id;
    bool operator>(const Enemy& other) const {
        return y > other.y;
    }
};

class SorterInMap {
    private:
        std::priority_queue<Enemy, std::vector<Enemy>, std::greater<Enemy>> pq;

        void push(Vect2D pos, int id) {
            pq.push({pos.x, pos.y, id});
        }
        
        int pop() {
            Enemy enemy = pq.top();
            pq.pop();
            return enemy.id;
        }

    public:
        std::vector<int> sort(EntitiesManager& manager) {
            auto entityIDs = manager.with<PositionOnMapComponent>().with<SizeComponent>().with<CollisionComponent>().get();
            for(auto id : entityIDs) {
                auto enemyPos = manager.getComponent<PositionOnMapComponent>(id).get()->point;
                if(manager.hasComponent<OriginComponent>(id)) {
                    auto origin = manager.getComponent<OriginComponent>(id).get()->shift;
                    enemyPos -= origin;
                }
                auto collShift = manager.getComponent<CollisionComponent>(id).get()->shiftFromLeftUp;

                auto leftUpP = enemyPos + collShift;
                push(leftUpP, id);
            }
            
            std::vector<int> res;
            while(!pq.empty()) res.push_back(pop());
            return res;
        }
};

void EntitiOnMapDrawSystem(
    sf::RenderWindow &window, TextureLoader &textureLoader, 
    EntitiesManager& manager
) { 
    // Получение id карты и игрока и enemy(на карте)
    auto mapIDs = manager.with<MapComponent>().get();
    auto playerIDs = manager.withClassName("*player*");
    if(mapIDs.size() == 0 || playerIDs.size() == 0) return;

    //Сортируем в порядке
    SorterInMap sorterInMap; 
    auto enemyIDs = sorterInMap.sort(manager);

    auto mapComponent = manager.getComponent<MapComponent>(mapIDs[0]).get();
    auto playerComponent = manager.getComponent<PositionOnMapComponent>(playerIDs[0]).get();

    // Получение резмеров и позиции видимой области
    Vect2D cameraPos = playerComponent->point;
    Vect2D visibleArea = mapComponent->visiableArea * 32;
    Vect2D leftUpP = cameraPos - visibleArea/2;
    Vect2D RightDownP = cameraPos + visibleArea/2;

    // Рисовка объектов
    for(auto enemyID : enemyIDs) {
        auto enemyPos = manager.getComponent<PositionOnMapComponent>(enemyID).get();
        auto size = manager.getComponent<SizeComponent>(enemyID).get()->size;
        if(enemyPos->point > leftUpP && enemyPos->point < RightDownP) {
            auto pos = enemyPos->point - cameraPos + Vect2D(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
            //std::cout << "Есть контакт: " << pos.x << " " << pos.y << "\n"; //отображение координат объекта который рисуется сейчас

            auto animation = manager.getComponent<AnimationGridComponent>(enemyID);
            auto state = manager.getComponent<StateComponent>(enemyID).get()->state;

            if(manager.hasComponent<OriginComponent>(enemyID)) {
                auto shift = manager.getComponent<OriginComponent>(enemyID).get()->shift;
                pos -= shift;
            }

            auto sprite = getCurrentSprite(animation.get(), pos, size, textureLoader, state);
            window.draw(sprite);
        }
    }
}