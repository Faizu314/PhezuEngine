#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/DataComponent.hpp"
#include "Scene/Components/PhysicsData.hpp"
#include "Scene/Factory/BlueprintInstantiator.hpp"
#include "Assets/Types/PrefabAsset.hpp"
#include "Assets/Systems/AssetManager.hpp"
#include "Scripting/Systems/ScriptEngine.hpp"

namespace Phezu {
    
    Scene::Scene(SceneContext ctx, const std::string& name) : m_Ctx(ctx), m_Name(name) { }
    
    Entity* Scene::CreateEntity() {
        Entity* entity = new Entity(this);
        
        m_RuntimeEntities.insert(std::make_pair(entity->GetEntityID(), entity));
        
        return entity;
    }
    
    Entity* Scene::CreateEntity(AssetHandle prefabHandle) {
        auto prefab = m_Ctx.assetManager->GetAsset<PrefabAsset>(prefabHandle);
        
        if (!prefab) {
            //TODO: add asserts
            return nullptr;
        }
        
        BlueprintRuntimeContext ctx = { m_Ctx.assetManager, m_Ctx.resourceManager, m_Ctx.scriptEngine, this};

        auto entity = BlueprintInstantiator::Instantiate(ctx, prefab->GetBlueprint(), prefabHandle);
        
        entity->RecalculateSubtreeTransforms();
                
        return entity;
    }
    
    Entity* Scene::GetEntity(uint64_t entityID) const {
        auto it = m_RuntimeEntities.find(entityID);
        return it != m_RuntimeEntities.end() ? it->second : nullptr;
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
        
        DestroyEntityInternal(it->second);
    }

    void Scene::DestroyEntityInternal(Entity* entity) {
        size_t childCount = entity->GetChildCount();

        for (size_t i = 0; i < childCount; i++) {
            if (auto child = entity->GetChild(i))
                DestroyEntityInternal(child);
        }

        m_Ctx.scriptEngine->OnEntityDestroyed(entity);
        entity->OnDestroyed();

        m_RuntimeEntities.erase(entity->GetEntityID());
        delete entity;
    }
    
    void Scene::LogicUpdate(float deltaTime) {
        for (auto entityID : m_EntitiesToDestroy)
            DestroyEntityInternal(entityID);
        m_EntitiesToDestroy.clear();
        
        UpdateHierarchy();
    }
    
    void Scene::UpdateHierarchy() {
        for (auto entity : m_RuntimeEntities) {
            if (entity.second->IsDirty()) {
                entity.second->RecalculateSubtreeTransforms();
            }
        }
    }
    
    void Scene::GetPhysicsEntities(std::vector<Entity*>& staticEntities, std::vector<Entity*>& dynamicEntities, size_t& staticIndex, size_t& dynamicIndex) const {
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            
            if (!entity->IsActive)
                continue;
            if (!entity->HasDataComponent(ComponentType::Physics) ||
                !entity->HasDataComponent(ComponentType::Shape))
                continue;
            
            auto physicsData = dynamic_cast<PhysicsData*>(entity->GetDataComponent(ComponentType::Physics));
            if (physicsData->IsStatic) {
                if (staticIndex < staticEntities.size())
                    staticEntities[staticIndex] = entity;
                else
                    staticEntities.push_back(entity);
                staticIndex++;
            }
            else if (!physicsData->IsStatic) {
                if (dynamicIndex < dynamicEntities.size())
                    dynamicEntities[dynamicIndex] = entity;
                else
                    dynamicEntities.push_back(entity);
                dynamicIndex++;
            }
        }
    }
    
    void Scene::GetRenderableEntities(std::vector<Entity*>& entities, size_t& index) const {
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            
            if (!entity->IsActive)
                continue;
            if (!entity->HasDataComponent(ComponentType::Render) ||
                !entity->HasDataComponent(ComponentType::Shape))
                continue;
            
            if (index < entities.size())
                entities[index] = entity;
            else
                entities.push_back(entity);
            index++;
        }
    }
    
    void Scene::BeginUnload() {

    }
    
    void Scene::Unload() {
        std::vector<Entity*> entities(m_RuntimeEntities.size());

        for (auto kvp : m_RuntimeEntities) {
            entities.push_back(kvp.second);
        }

        for (auto entity : entities) {
            DestroyEntityInternal(entity);
        }
    }
}
