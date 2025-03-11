#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "scene/components/DataComponent.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/BehaviourComponent.hpp"
#include "scene/Prefab.hpp"
#include "Engine.hpp"

#include <stdexcept>

namespace Phezu {
    
    Scene::Scene(Engine* engine) : m_Engine(engine), m_IsLoaded(false) { }
    Scene::Scene(Engine* engine, const std::string& name) : m_Engine(engine), m_Name(name), m_IsLoaded(false) { }
    
    std::weak_ptr<Entity> Scene::CreateEntity() {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(shared_from_this());
        m_RuntimeEntities.insert(std::make_pair(entity->GetEntityID(), entity));
        
        return entity;
    }
    
    std::weak_ptr<Entity> Scene::CreateEntity(uint64_t prefabID) {
        auto prefab = m_Engine->GetPrefab(prefabID).lock();
        
        if (!prefab) {
            //TODO: logging
            return std::weak_ptr<Entity>();
        }
        
        auto entity = prefab->Instantiate(shared_from_this())[0];
        m_RuntimeEntities.insert(std::make_pair(entity->GetEntityID(), entity));
        
        return entity;
    }
    
    std::weak_ptr<Entity> Scene::GetEntity(uint64_t entityID) const {
        try {
            return m_RuntimeEntities.at(entityID);
        }
        catch (const std::out_of_range&) {
            return std::weak_ptr<Entity>();
        }
    }
    
    void Scene::DestroyEntity(uint64_t entityID) {
        if (std::find(m_EntitiesToDestroy.begin(), m_EntitiesToDestroy.end(), entityID) != m_EntitiesToDestroy.end())
            return;
        
        m_EntitiesToDestroy.push_back(entityID);
    }
    
    void Scene::DestroyEntityInternal(uint64_t entityID) {
        auto it = m_RuntimeEntities.find(entityID);
        
        if (it == m_RuntimeEntities.end())
            return;
        
        size_t childCount = it->second->GetChildCount();
        
        for (size_t i = 0; i < childCount; i++) {
            if (auto child = it->second->GetChild(i).lock())
                DestroyEntityInternal(child->GetEntityID());
        }
        
        for (auto comp : it->second->m_BehaviourComponents)
            comp->OnDestroy();
        it->second->OnDestroyed();
        
        m_RuntimeEntities.erase(it);
    }
    
    void Scene::Load() {
        auto createdEntities = m_SceneEntities.Instantiate(shared_from_this());
        
        for (size_t i = 0; i < createdEntities.size(); i++) {
            m_RuntimeEntities.insert(std::make_pair(createdEntities[i]->GetEntityID(), createdEntities[i]));
        }
        
        m_IsLoaded = true;
        
        for (auto [id, entity] : m_RuntimeEntities) {
            if (entity->IsDirty()) {
                entity->RecalculateSubtreeTransformations();
            }
        }
    }
    
    void Scene::LogicUpdate(float deltaTime) {
        for (auto entityID : m_EntitiesToDestroy)
            DestroyEntityInternal(entityID);
        
        m_EntitiesToDestroy.clear();
        
        for (auto entity : m_RuntimeEntities) {
            for (auto comp : entity.second->GetComponents<BehaviourComponent>()) {
                comp.lock()->Update(deltaTime);
                
                if (!m_IsLoaded)
                    return;
            }
        }
        
        UpdateHierarchy();
    }
    
    void Scene::UpdateHierarchy() {
        for (auto entity : m_RuntimeEntities) {
            if (entity.second->IsDirty()) {
                entity.second->RecalculateSubtreeTransformations();
            }
        }
    }
    
    void Scene::GetPhysicsEntities(std::vector<std::weak_ptr<Entity>>& entities, size_t& staticCount, size_t& dynamicCount) const {
        staticCount = dynamicCount = 0;
        
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            auto physicsData = entity->GetPhysicsData().lock();
            if (physicsData && physicsData->IsStatic() && entity->GetShapeData() != nullptr) {
                if (staticCount < entities.size())
                    entities[staticCount] = entity;
                else
                    entities.push_back(entity);
                staticCount++;
            }
        }
        
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            auto physicsData = entity->GetPhysicsData().lock();
            if (physicsData && !physicsData->IsStatic() && entity->GetShapeData() != nullptr) {
                if (staticCount + dynamicCount < entities.size())
                    entities[staticCount + dynamicCount] = entity;
                else
                    entities.push_back(entity);
                dynamicCount++;
            }
        }
    }
    
    void Scene::GetRenderableEntities(std::vector<std::weak_ptr<Entity>>& entities, size_t& count) const {
        count = 0;
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            if (entity->GetRenderData() != nullptr && entity->GetShapeData() != nullptr) {
                if (count < entities.size())
                    entities[count] = entity;
                else
                    entities.push_back(entity);
                count++;
            }
        }
    }
    
    void Scene::BeginUnload() {
        m_IsLoaded = false;
    }
    
    void Scene::Unload() {
        m_RuntimeEntities.clear();
        m_IsLoaded = false;
    }
    
    long long unsigned int Scene::GetFrameCount() const {
        return m_Engine->GetFrameCount();
    }
    
    std::string Scene::Serialize() const {
        nlohmann::json j;
        
        j["Name"] = m_Name;
        m_SceneEntities.Serialize(j);
    }
    
    void Scene::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        m_Name = j["Name"].get<std::string>();
        m_SceneEntities.Deserialize(j);
    }
}
