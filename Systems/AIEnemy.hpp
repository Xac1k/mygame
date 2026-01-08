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
#include <Map/Creating/Common/randomRange.hpp>
#include <Entities/components/items.hpp>
#include <Common/randFloat.hpp>
#include <cmath>
#include <Common/normAngle.hpp>
#include <Common/getEntitiesByChunk.hpp>

class AIAgent {
    public:
        int MAX_COOLDOWN_BETWEEN_WANDERING = 20;
        int MIN_COOLDOWN_BETWEEN_WANDERING = 5;
        int TRESHHOLD_POINT_DONE = TILE_SIZE;

        
        void updateAgressiveEnemy(EntitiesManager& manager, float df) {
            auto enemyIDsWithAgressiveAI = manager.withClassName("*AIEnemy:agressive*");
            if(enemyIDsWithAgressiveAI.size() == 0) return;

            for (int enemyID : enemyIDsWithAgressiveAI) {
                auto AIComp = manager.getComponent<AIAgentCompanent>(enemyID).get();
                switch (AIComp->mode)
                {
                    case HowToFindTarget::ByClassName:
                        updateAgresiveEnemyByClassName(manager, AIComp, enemyID, df);
                        break;

                    case HowToFindTarget::ByID:
                        updateAgresiveEnemyByID(manager, AIComp, enemyID, df);
                        break;
                
                default:
                    break;
                }
            }
        }
    private:
        //common methods of handling AI
        void updateAgresiveEnemyByClassName(EntitiesManager& manager, AIAgentCompanent *AIComp, int enemyID, float df) {
            auto posAgressor = manager.getComponent<PositionOnMapComponent>(enemyID).get()->point;
            auto targetEnemyIDs = manager.with<PositionOnMapComponent>().except(enemyID).withClassName(AIComp->classNameOfTarget).get();
            targetEnemyIDs = sortByDeath(manager, targetEnemyIDs);
            
            if(!ChasingLoop(manager, AIComp, enemyID, targetEnemyIDs, posAgressor))
                WanderingLoop(manager, AIComp, enemyID, posAgressor, df);
        }

        void updateAgresiveEnemyByID(EntitiesManager& manager, AIAgentCompanent *AIComp, int enemyID, float df) {
        }



        // define goal to movment
        bool ChasingLoop(EntitiesManager& manager, AIAgentCompanent *AIComp, int enemyID, std::vector<int> targetEnemyIDs, Vect2D posAgressor) {
            for(int targetEnemyID : targetEnemyIDs) {
                auto posTarget = manager.getComponent<PositionOnMapComponent>(targetEnemyID).get()->point;
                if(dist(posAgressor, posTarget) < AIComp->detectionRadius) {
                    if(dist(posAgressor, posTarget) < AIComp->attackRadius)
                        AttackLoop(manager, AIComp, enemyID, posAgressor, posTarget, targetEnemyID);
                    else 
                        defineMovmentToTarget(manager, AIComp, enemyID, posTarget);
                    return true;
                }
            }
            return false;
        }


        void AttackLoop(EntitiesManager& manager, AIAgentCompanent *AIComp, int enemyID, Vect2D posAgressor, Vect2D posTarget, int targetEnemyID) {
            AIComp->state = AIState::Attacking;

            // stop
            manager.removeComponent<WanderPointCompanent>(enemyID); 
            auto veloComp = manager.getComponent<VelocityComponent>(enemyID).get();
            veloComp->dir = Vect2D(0, 0); 

            // prepare
            if(!manager.hasComponent<ReadyToAttack>(enemyID)) {
                PrepareToAttack(manager, enemyID);
                return;
            }

            // remove flag
            manager.removeComponent<ReadyToAttack>(enemyID);

            // attack
            addAttackComp(manager, enemyID, posTarget, posAgressor);
        }

        void PrepareToAttack(EntitiesManager& manager, int enemyID) {
            if(manager.hasComponent<ColldownBeforeAttackComp>(enemyID)) return;
            if(manager.hasComponent<AttackCooldownCompanent>(enemyID)) return;
        
            //info
            auto cooldownInfo = manager.getComponent<CooldownInfo>(enemyID).get();

            std::cout << "Ставим задержку до атаки на " << cooldownInfo->cooldownBeforeAttack << std::endl; 

            //prepare set cooldown
            ColldownBeforeAttackComp cooldownBeforeAttack(cooldownInfo->cooldownBeforeAttack);
            manager.addComponent(cooldownBeforeAttack, enemyID);
        }


        void defineMovmentToTarget(EntitiesManager& manager, AIAgentCompanent *AIComp, int enemyID, Vect2D posTarget) {
            AIComp->state = AIState::Chasing;

            // clear all cooldown Attack
            manager.removeComponent<ColldownBeforeAttackComp>(enemyID);
            manager.removeComponent<AttackCooldownCompanent>(enemyID);
            
            //set wander point
            if(manager.hasComponent<WanderPointCompanent>(enemyID)) {
                auto wanderingPoint = manager.getComponent<WanderPointCompanent>(enemyID).get();
                wanderingPoint->cooldown = 0;
                wanderingPoint->currTime = 0;
                wanderingPoint->point = posTarget;
            }
            else {
                auto goal = getAttackGoal(AIComp, posTarget);
                manager.addComponent(goal, enemyID);
            }
        }

        WanderPointCompanent getAttackGoal(AIAgentCompanent *AIComp, Vect2D posTarget) {
            WanderPointCompanent wanderPoint;
            wanderPoint.cooldown = 0;
            wanderPoint.currTime = 0;
            wanderPoint.point = posTarget;

            return wanderPoint;
        }

        void addAttackComp(EntitiesManager& manager, int ID, Vect2D posTarget, Vect2D posAgressor) {
            auto angle = getAngle(posTarget - posAgressor);
            auto playerPos = manager.getComponent<PositionOnMapComponent>(ID).get()->point;
            auto weaponComp = manager.getComponent<WeaponComponent>(ID).get();

            Vect2D attackArea(normAngle(angle - weaponComp->deviation), normAngle(angle + weaponComp->deviation));
            AttackComponent attack(weaponComp, playerPos, attackArea);
            if(weaponComp->haveEffect()) attack.moveEffect(weaponComp);

            StuneCompanent stune(weaponComp->cooldownMoving);
            AttackCooldownCompanent AttackCooldown(weaponComp->cooldown);
            std::cout << "Мобы: Ставим задержку до атаки на " << weaponComp->cooldown << std::endl; 
            manager.addComponent<AttackComponent>(attack, ID);
            manager.addComponent<StuneCompanent>(stune, ID);
            manager.addComponent<AttackCooldownCompanent>(AttackCooldown, ID);
        }


        // randomize goal 
        void WanderingLoop(EntitiesManager& manager, AIAgentCompanent *AIComp, int enemyID, Vect2D posAgressor, float df) {
            AIComp->state = AIState::Wandering;

            if(manager.hasComponent<WanderPointCompanent>(enemyID)) {
                auto targetPoint = manager.getComponent<WanderPointCompanent>(enemyID).get();
                if(
                    abs(targetPoint->point.x - posAgressor.x) < TRESHHOLD_POINT_DONE &&
                    abs(targetPoint->point.y - posAgressor.y) < TRESHHOLD_POINT_DONE
                ) {
                    //goal done
                    if(targetPoint->currTime < targetPoint->cooldown) {
                        targetPoint->currTime += df;   
                    }
                    else { 
                        WanderPointCompanent wanderPoint = getWanderGoal(AIComp, posAgressor);
                        targetPoint->currTime = 0;
                        targetPoint->cooldown = wanderPoint.cooldown;
                        targetPoint->point = wanderPoint.point;
                    }
                }
            }
            else {
                WanderPointCompanent wanderPoint = getWanderGoal(AIComp, posAgressor);
                manager.addComponent<WanderPointCompanent>(wanderPoint, enemyID);
            }
        }

        WanderPointCompanent getWanderGoal(AIAgentCompanent *AIComp, Vect2D posAgressor) {
            float angle = randFloat() * 360 / 180 * M_PI;
            int dist = randRange(10, AIComp->wanderRadius);

            Vect2D movmentVect(dist * cos(angle), dist * sin(angle));
            WanderPointCompanent wanderPoint;
            wanderPoint.cooldown = (float)randRange(MIN_COOLDOWN_BETWEEN_WANDERING, MAX_COOLDOWN_BETWEEN_WANDERING);
            wanderPoint.currTime = 0;
            wanderPoint.point = posAgressor + movmentVect;

            return wanderPoint;
        }



    public:
        // define Velocity for AI enemy
        void defineEnemyVelocityByWandering(EntitiesManager& manager) {
            //find alive entities
            auto enemyIDs = manager.with<WanderPointCompanent>().with<DeathComponent>().get();
            if(enemyIDs.size() == 0) return;
            enemyIDs = sortByDeath(manager, enemyIDs);

            //handler
            for(int enemyID : enemyIDs) {
                // not attack
                auto AIComp = manager.getComponent<AIAgentCompanent>(enemyID).get();
                if(AIComp->state == AIState::Attacking) continue;

                auto posComp = manager.getComponent<PositionOnMapComponent>(enemyID).get()->point;
                auto wanderPoint = manager.getComponent<WanderPointCompanent>(enemyID).get();
                auto velocityComp = manager.getComponent<VelocityComponent>(enemyID).get();

                // stune handler for enemy with AI
                if(manager.hasComponent<StuneCompanent>(enemyID)) {
                    velocityComp->dir = Vect2D(0, 0);
                    continue;
                }

                // movment
                Vect2D movmentVect = wanderPoint->point - posComp;
                if(AIComp->state == AIState::Wandering)
                    velocityComp->dir = movmentVect / movmentVect.length() * AIComp->speedOfWalk;
                if(AIComp->state == AIState::Chasing)
                    velocityComp->dir = movmentVect / movmentVect.length() * AIComp->speedOfChasing;

                // dist < treshold dist
                if(!std::isfinite(velocityComp->dir.x))
                    velocityComp->dir.x = 0;
                if(!std::isfinite(velocityComp->dir.y))
                    velocityComp->dir.y = 0;
            }
        }

        // define next Pos for all enemy
        void updateEnemyPos(EntitiesManager& manager, float df) {
            auto enemyIDs = manager.with<VelocityComponent>().get();
            if(enemyIDs.size() == 0) return;

            auto mapIds = manager.with<MapComponent>().get();
            if(mapIds.size() == 0) return;
            auto map = manager.getComponent<MapComponent>(mapIds[0]).get();

            for(int enemyID : enemyIDs) {
                auto velocityComp = manager.getComponent<VelocityComponent>(enemyID).get();
                if(velocityComp->dir.x == 0 && velocityComp->dir.y == 0) continue;
                auto posComp = manager.getComponent<PositionOnMapComponent>(enemyID).get();
                auto collisionRectComp = manager.getComponent<CollisionComponent>(enemyID).get();
                auto originComp = manager.getComponent<OriginComponent>(enemyID).get();

                // stune handler
                if(manager.hasComponent<StuneCompanent>(enemyID)) {
                    velocityComp->dir = Vect2D(0, 0);
                    continue;
                }

                // permissions
                auto [xCoef, yCoef] = isPermittedByMap(map, posComp, collisionRectComp, originComp, velocityComp, df);
                auto [xCoefEnemy, yCoefEnemy] = isPermittedByEnemy(manager, enemyID, posComp, collisionRectComp, originComp, velocityComp, df);
                velocityComp->dir = velocityComp->dir * Vect2D(xCoef * xCoefEnemy, yCoef * yCoefEnemy);

                // step
                posComp->point += velocityComp->dir * df;
            }
        }

    // permissions handlers
    private:
        enum class Direction {
            left, 
            right, 
            down, 
            up,
            leftUp, 
            leftDown, 
            rightUp, 
            rightDown,
            none
        };

        Direction getDirection(Vect2D vect) {
            if(vect.x < 0 && vect.y > 0) return Direction::leftDown;
            if(vect.x > 0 && vect.y > 0) return Direction::rightDown;
            if(vect.x < 0 && vect.y < 0) return Direction::leftUp;
            if(vect.x > 0 && vect.y < 0) return Direction::rightUp;
            if(vect.x < 0) return Direction::left;
            if(vect.x > 0) return Direction::right;
            if(vect.y < 0) return Direction::up;
            if(vect.y > 0) return Direction::down;
            return Direction::none;
        }

        Direction getDirLeftRight(Vect2D vect) {
            if(vect.x < 0) return Direction::left;
            if(vect.x > 0) return Direction::right;
            if(vect.y < 0) return Direction::left;
            if(vect.y > 0) return Direction::right;
            return Direction::none;
        }

        std::tuple<int, int> isPermittedByMap(MapComponent* map, PositionOnMapComponent* enemyPos, CollisionComponent* rect, OriginComponent* origin, VelocityComponent* velo, float df) {
            auto collRectLeftUp = enemyPos->point + rect->shiftFromLeftUp + velo->dir * df;
            if(origin) collRectLeftUp -= origin->shift;
            sf::Vector2i Left = {(int)collRectLeftUp.x, (int)(collRectLeftUp.y + rect->size.y/2)};
            sf::Vector2i LeftUp = {(int)collRectLeftUp.x, (int)collRectLeftUp.y};
            sf::Vector2i Up = {(int)(collRectLeftUp.x + rect->size.x/2), (int)collRectLeftUp.y};
            sf::Vector2i RigthUp = {(int)(collRectLeftUp.x + rect->size.x), (int)collRectLeftUp.y};
            sf::Vector2i Rigth = {(int)(collRectLeftUp.x + rect->size.x), (int)(collRectLeftUp.y + rect->size.y/2)};
            sf::Vector2i RigthDown = {(int)(collRectLeftUp.x + rect->size.x), (int)(collRectLeftUp.y + rect->size.y)};
            sf::Vector2i Down = {(int)(collRectLeftUp.x + rect->size.x/2), (int)(collRectLeftUp.y + rect->size.y)};
            sf::Vector2i LeftDown = {(int)(collRectLeftUp.x), (int)(collRectLeftUp.y + rect->size.y)};

            auto dir = getDirection(velo->dir);

            bool leftColl = map->map.get(Left.x/TILE_SIZE, Left.y/TILE_SIZE).type != TileType::Floor;
            bool leftUpColl = map->map.get(LeftUp.x/TILE_SIZE, LeftUp.y/TILE_SIZE).type != TileType::Floor;
            bool UpColl = map->map.get(Up.x/TILE_SIZE, Up.y/TILE_SIZE).type != TileType::Floor;
            bool RigthUpColl = map->map.get(RigthUp.x/TILE_SIZE, RigthUp.y/TILE_SIZE).type != TileType::Floor;
            bool RigthColl = map->map.get(Rigth.x/TILE_SIZE, Rigth.y/TILE_SIZE).type != TileType::Floor;
            bool RigthDownColl = map->map.get(RigthDown.x/TILE_SIZE, RigthDown.y/TILE_SIZE).type != TileType::Floor;
            bool DownColl = map->map.get(Down.x/TILE_SIZE, Down.y/TILE_SIZE).type != TileType::Floor;
            bool LeftDownColl = map->map.get(LeftDown.x/TILE_SIZE, LeftDown.y/TILE_SIZE).type != TileType::Floor;
            
            if(leftColl || leftUpColl || UpColl || RigthUpColl || RigthColl || RigthDownColl || DownColl || LeftDownColl)
            {
                if(dir == Direction::leftUp && UpColl && !leftColl) return std::make_tuple<int, int>(1, 0);
                if(dir == Direction::leftUp && RigthUpColl && !leftColl) return std::make_tuple<int, int>(1, 0);
                if(dir == Direction::rightUp && UpColl && !RigthColl) return std::make_tuple<int, int>(1, 0);
                if(dir == Direction::rightUp && leftUpColl && !RigthColl) return std::make_tuple<int, int>(1, 0);

                if(dir == Direction::leftUp && leftColl && !UpColl) return std::make_tuple<int, int>(0, 1);
                if(dir == Direction::leftUp && LeftDownColl && !UpColl) return std::make_tuple<int, int>(0, 1);
                if(dir == Direction::leftDown && leftUpColl && !DownColl) return std::make_tuple<int, int>(0, 1);
                if(dir == Direction::leftDown && leftColl && !DownColl) return std::make_tuple<int, int>(0, 1);

                if(dir == Direction::rightDown && LeftDownColl && !RigthColl) return std::make_tuple<int, int>(1, 0);
                if(dir == Direction::leftDown && RigthDownColl && !leftColl) return std::make_tuple<int, int>(1, 0);

                if(dir == Direction::rightUp && RigthDownColl && !UpColl) return std::make_tuple<int, int>(0, 1);
                if(dir == Direction::rightDown && RigthUpColl && !DownColl) return std::make_tuple<int, int>(0, 1);
                return std::make_tuple<int, int>(0, 0);
            }
            return std::make_tuple<int, int>(1, 1);
        }

        std::vector<int> sortByDeath(EntitiesManager& manager, std::vector<int> entityIDs) {
            std::vector<int> result;
            for(int id : entityIDs) {
                auto deathComp = manager.getComponent<DeathComponent>(id).get();
                if(!deathComp->isDead) result.push_back(id);
            }
            return result;
        }

        ArchiveOfEntitiesInChunk ChunkManager;
        std::tuple<int, int> isPermittedByEnemy(
            EntitiesManager& manager, int moverID, PositionOnMapComponent* moverPos, CollisionComponent* moverCollRect,
            OriginComponent* origin, VelocityComponent* velo, float df
        ) {
            auto moverCollRectLeftUp = moverPos->point + moverCollRect->shiftFromLeftUp;
            if(origin) moverCollRectLeftUp -= origin->shift;
            int xAllow = 1; int yAllow = 1;

            auto enemyIDsInChunk = ChunkManager.getEntitiesByChunk(manager);
            auto enemyIDs = manager.load<PositionOnMapComponent>(enemyIDsInChunk).with<CollisionComponent>().except(moverID).withClassName("*Enemy*").get();
            enemyIDs = sortByDeath(manager, enemyIDs);
            
            for (int enemyID : enemyIDs) {
                auto enemyPos = manager.getComponent<PositionOnMapComponent>(enemyID).get();
                auto enemySize = manager.getComponent<SizeComponent>(enemyID).get();
                auto enemyCollRect = manager.getComponent<CollisionComponent>(enemyID).get();
                Vect2D origin({0, 0});
                if(manager.hasComponent<OriginComponent>(enemyID))
                    origin = manager.getComponent<OriginComponent>(enemyID).get()->shift;
                auto enemyCollRectLeftUp = enemyPos->point - origin + enemyCollRect->shiftFromLeftUp;
                if(boxesOverlap(moverCollRectLeftUp + Vect2D(velo->dir.x * df, 0), moverCollRect->size, enemyCollRectLeftUp, enemyCollRect->size))
                    xAllow = 0;
                if(boxesOverlap(moverCollRectLeftUp + Vect2D(0, velo->dir.y * df), moverCollRect->size, enemyCollRectLeftUp, enemyCollRect->size)) 
                    yAllow = 0;
                if(xAllow == 0 && yAllow == 0) 
                    return std::make_pair(0, 0);
            }
            return std::make_pair(xAllow, yAllow);
        }
};