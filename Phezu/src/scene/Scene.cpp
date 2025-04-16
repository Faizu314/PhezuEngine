#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "scene/components/DataComponent.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/ScriptComponent.hpp"
#include "scene/Prefab.hpp"
#include "Engine.hpp"

#include <stdexcept>

namespace Phezu {
    
    Scene::Scene(Engine* engine) : m_Engine(engine), m_IsLoaded(false) { }
    Scene::Scene(Engine* engine, const std::string& name) : m_Engine(engine), m_Name(name), m_IsLoaded(false) { }
    
    std::weak_ptr<Entity> Scene::CreateEntity() {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(this);
        m_RuntimeEntities.insert(std::make_pair(entity->GetEntityID(), entity));
        
        return entity;
    }
    
    std::weak_ptr<Entity> Scene::CreateEntity(GUID prefabGuid) {
        auto prefab = m_Engine->GetPrefab(prefabGuid).lock();
        
        if (!prefab) {
            //TODO: logging
            return std::weak_ptr<Entity>();
        }
        
        auto entity = prefab->Instantiate(this);
        
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
        
        m_Engine->GetScriptEngine().OnEntityDestroyed(it->second);
        it->second->OnDestroyed();
        
        m_RuntimeEntities.erase(it);
    }
    
    void Scene::Load() {
        m_SceneEntities.Instantiate(this);
        
        ScriptEngine& scriptEngine = m_Engine->GetScriptEngine();

        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            scriptEngine.OnEntityCreated(it->second);
        }

        m_IsLoaded = true;
        
        UpdateHierarchy();
    }
    
    void Scene::LogicUpdate(float deltaTime) {
        for (auto entityID : m_EntitiesToDestroy)
            DestroyEntityInternal(entityID);
        m_EntitiesToDestroy.clear();

        m_Engine->GetScriptEngine().OnUpdate(deltaTime);
        
        UpdateHierarchy();
    }
    
    void Scene::UpdateHierarchy() {
        for (auto entity : m_RuntimeEntities) {
            if (entity.second->IsDirty()) {
                entity.second->RecalculateSubtreeTransformations();
            }
        }
    }
    
    void Scene::GetPhysicsEntities(std::vector<std::weak_ptr<Entity>>& staticEntities, std::vector<std::weak_ptr<Entity>>& dynamicEntities, size_t& staticIndex, size_t& dynamicIndex) const {
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            auto physicsData = entity->GetPhysicsData();
            if (physicsData && physicsData->IsStatic() && entity->GetShapeData() != nullptr) {
                if (staticIndex < staticEntities.size())
                    staticEntities[staticIndex] = entity;
                else
                    staticEntities.push_back(entity);
                staticIndex++;
            }
            else if (physicsData && !physicsData->IsStatic() && entity->GetShapeData() != nullptr) {
                if (dynamicIndex < dynamicEntities.size())
                    dynamicEntities[dynamicIndex] = entity;
                else
                    dynamicEntities.push_back(entity);
                dynamicIndex++;
            }
        }
    }
    
    void Scene::GetRenderableEntities(std::vector<std::weak_ptr<Entity>>& entities, size_t& index) const {
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            if (entity->GetRenderData() != nullptr && entity->GetShapeData() != nullptr) {
                if (index < entities.size())
                    entities[index] = entity;
                else
                    entities.push_back(entity);
                index++;
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
        j["Guid"] = m_Guid.Value;
        m_SceneEntities.Serialize(j);

        return j.dump(4);
    }
    
    void Scene::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        m_Name = j["Name"].get<std::string>();
        m_Guid.Value = j["Guid"].get<uint64_t>();
        m_SceneEntities.Deserialize(j);
        m_SceneEntities.Initialize(m_Engine, m_Guid);
    }
}
