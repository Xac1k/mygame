#pragma once
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include <functional>
#include <Common/regex.hpp>
#include "component.hpp"

template<typename T>
using MapEntities = std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<T>>>;

template<typename... ComponentTypes>
class EntityQuery {
private:
    std::vector<int> entityIDs = {};
    MapEntities<void> entities = {};
    std::map<std::string, int> existedEntities;

    bool isValid(int entityID) {
        for (auto id : entityIDs) {
            if(id == entityID) return true;
        }
        return false;
    }

public:
    EntityQuery(std::vector<int> ids, MapEntities<void>& entitiesI, std::map<std::string, int>& existedEntitiesI) 
        : entityIDs(ids), entities(entitiesI), existedEntities(existedEntitiesI) {};

    template<typename NewComponent>
    EntityQuery<ComponentTypes..., NewComponent> with() {
        std::vector<int> result;
        for (int id : entityIDs) {
            if(hasComponent<NewComponent>(id)) {
                result.push_back(id);
            }
        }
        return EntityQuery<ComponentTypes..., NewComponent>(result, entities, existedEntities);
    }

    template<typename NewComponent>
    EntityQuery<ComponentTypes..., NewComponent> without() {
        std::vector<int> result;
        for (int id : entityIDs) {
            if(!hasComponent<NewComponent>(id)) {
                result.push_back(id);
            }
        }
        return EntityQuery<ComponentTypes..., NewComponent>(result, entities, existedEntities);
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

    EntityQuery<ComponentTypes...> except(int ID) {
        std::vector<int> result;
        for(auto entityID : entityIDs) {
            if(entityID != ID) result.push_back(entityID);
        }
        return EntityQuery<ComponentTypes...>(result, entities, existedEntities);
    }

    std::vector<int> get() const {
        return entityIDs;
    }


    std::vector<int> withClassName(std::string pattern) {
        std::vector<int> result;
        for (auto className : existedEntities) {
            if(regexmatch(pattern, className.first) && isValid(className.second)) {
                result.push_back(className.second);
            }
        }
        return result;
    }
};

class EntitiesManager
{
private:
    MapEntities<void> entities = {};
    std::map<std::string, int> existedEntities;

    int id = 0;
public:
    void addEntity(int entityID) {
        if (entities.find(entityID) != entities.end()) return;
        entities[entityID] = std::unordered_map<std::type_index, std::shared_ptr<void>>();
        id++;
    }

    void addEntity() {
        entities[id] = std::unordered_map<std::type_index, std::shared_ptr<void>>();
        existedEntities[std::to_string(id)] = id;
        id++;
    }

    void addEntity(std::string className, bool useID=false) {
        entities[id] = std::unordered_map<std::type_index, std::shared_ptr<void>>();
        if(useID) className += ":" + std::to_string(id);
        existedEntities[className] = id;
        id++;
    }

    void setClassName(int entityID, std::string newClassName, bool useID) {
        std::string className = getClassEntity(entityID);
        existedEntities.erase(className);
        if(useID) newClassName += ":" + std::to_string(id);
        existedEntities[newClassName] = entityID;
    }

    int getID() {
        return id-1;
    }

    void removeEntity(int entityID) {
        existedEntities.erase(getClassEntity(entityID));
        entities.erase(entityID);
    };

    void removeEntityByClass(std::string pattern) {
        std::vector<int> deletionIDs;
        for (auto className : existedEntities) {
            if(regexmatch(pattern, className.first)) {
                deletionIDs.push_back(className.second);
            }
        }
        for(int id : deletionIDs) {
            removeEntity(id);
        }
    }

    std::string getClassEntity(int entityID) {
        std::string className;
        for (auto it : existedEntities) {
            if(it.second == entityID) {
                className = it.first;
                break;
            }
        }
        return className;
    }

    bool isEntityExist(std::string className) {
        auto it = existedEntities.find(className);
        return it != existedEntities.end();
    }

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
    void addComponent(T& component, int entityID) {
        auto entity = entities.find(entityID);
        if (entity == entities.end()) return;
        entity->second[typeid(T)] = std::make_shared<T>(component);
    }

    template <typename T>
    void removeComponent(int entityID) {
        auto entity = entities.find(entityID);
        if (entity == entities.end()) return;
        entity->second.erase(typeid(T));
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
        return EntityQuery<T>(result, entities, existedEntities);
    }

    std::vector<int> withClassName(std::string pattern) {
        std::vector<int> result;
        for (auto className : existedEntities) {
            if(regexmatch(pattern, className.first)) {
                result.push_back(className.second);
            }
        }
        return result;
    }
};