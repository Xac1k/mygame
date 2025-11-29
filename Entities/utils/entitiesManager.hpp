#pragma once
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include <functional>
#include "component.hpp"

template<typename T>
using MapEntities = std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<T>>>;

template<typename... ComponentTypes>
class EntityQuery {
private:
    std::vector<int> entityIDs = {};
    MapEntities<void> entities = {};
public:
    EntityQuery(std::vector<int> ids, MapEntities<void>& entitiesI) 
        : entityIDs(ids), entities(entitiesI) {};

    template<typename NewComponent>
    EntityQuery<ComponentTypes..., NewComponent> with() {
        std::vector<int> result;
        for (int id : entityIDs) {
            if(hasComponent<NewComponent>(id)) {
                result.push_back(id);
            }
        }
        return EntityQuery<ComponentTypes..., NewComponent>(result, entities);
    }

    template<typename T>
    std::shared_ptr<T> getComponent(int entityID) {
        auto entity = entities.find(entityID);
        if (entity == entities.end()) return nullptr;
        
        auto component = entity->second.find(typeid(T));
        if (component == entity->second.end()) return nullptr;
        
        return std::static_pointer_cast<T>(component->second);
    }

    template<typename T>
    bool hasComponent(int entityID) {
        auto entity = entities.find(entityID);
        if (entity == entities.end()) return false;

        auto component = entity->second.find(typeid(T));
        return component != entity->second.end();
    }
    
    void forEach(std::function<void(int, std::shared_ptr<ComponentTypes>...)> callback) {
        for (int id : entityIDs) {
            callback(id, getComponent<ComponentTypes>(id)...);
        }
    }

    std::vector<int> get() const {
        return entityIDs;
    }
};

class EntitiesManager
{
private:
    MapEntities<void> entities = {};
    int id = 0;
public:
    void addEntity(int entityID) {
        if (entities.find(entityID) != entities.end()) return;
        entities[entityID] = std::unordered_map<std::type_index, std::shared_ptr<void>>();
        id++;
    }

    void addEntity() {
        entities[id] = std::unordered_map<std::type_index, std::shared_ptr<void>>();
        id++;
    }

    int getID() {
        return id-1;
    }

    void clearEntities() {
        entities.clear();
    }

    void removeEntity(int entityID) {
        entities.erase(entityID);
    };

    template <typename ...T>
    void addComponent(int entityID, std::function<void(T...)> callback) {
        auto entity = entities.find(entityID);
        if (entity == entities.end()) return;
        
        entity->second[typeid(CallBackComponent<T...>)] = std::make_shared<CallBackComponent<T...>>(callback);
    }
    
    template <typename T>
    void addComponent(int entityID, T& component) {
        auto entity = entities.find(entityID);
        if (entity == entities.end()) return;
        entity->second[typeid(T)] = std::make_shared<T>(component);
    }

    template <typename T>
    void addComponent(T& component) {
        auto entity = entities.find(id-1);
        if (entity == entities.end()) return;
        entity->second[typeid(T)] = std::make_shared<T>(component);
    }

    template <typename T>
    std::shared_ptr<T> getComponent(int entityID) {
        auto entity = entities.find(entityID);
        if (entity == entities.end()) return nullptr;
        
        auto component = entity->second.find(typeid(T));
        if (component == entity->second.end()) return nullptr;
        
        return std::static_pointer_cast<T>(component->second);
    }

    template <typename T>
    bool hasComponent(int entityID) {
        auto entity = entities.find(entityID);
        if (entity == entities.end()) return false;

        auto component = entity->second.find(typeid(T));
        return component != entity->second.end();
    }

    template <typename T>
    EntityQuery<T> with() {
        std::vector<int> result;
        for(const auto& pair : entities) {
            if (hasComponent<T>(pair.first)) {
                result.push_back(pair.first);
            }
        }
        return EntityQuery<T>(result, entities);
    }
};