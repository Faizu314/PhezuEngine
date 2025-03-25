#include "scene/Blueprint.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "nlohmann/json.hpp"
#include "scene/Prefab.hpp"
#include "Engine.hpp"

template <>
struct std::hash<Phezu::RegistryKey> {
    std::size_t operator()(const Phezu::RegistryKey& key) const noexcept {
        std::size_t h1 = std::hash<uint64_t>{}(key.InstanceID);
        std::size_t h2 = std::hash<uint64_t>{}(key.PrefabGuid);
        return h1 ^ (h2 << 1);
    }
};

namespace Phezu {
    
    RegistryKey::RegistryKey(uint64_t instanceID, uint64_t prefabGuid) : InstanceID(instanceID), PrefabGuid(prefabGuid) {}
    
    bool RegistryKey::operator==(const RegistryKey& other) const {
        return InstanceID == other.InstanceID && PrefabGuid == other.PrefabGuid;
    }
    
    bool RegistryKey::operator<(const RegistryKey& other) const {
        if (InstanceID == other.InstanceID)
            return PrefabGuid < other.PrefabGuid;
        return InstanceID < other.InstanceID;
    }
    
    
    void Blueprint::Serialize(nlohmann::json &j) const {
        j["Entries"] = nlohmann::json::array();
        
        for (const auto& entry : Entries) {
            nlohmann::json entryJson;
            entry.Serialize(entryJson);
            j["Entries"].push_back(entryJson);
        }
    }
    
    void Blueprint::Deserialize(const nlohmann::json& j) {
        Entries.clear();
        
        for (const auto& entryJson : j["Entries"]) {
            BlueprintEntry entry;
            entry.Deserialize(entryJson);
            Entries.push_back(entry);
        }
    }
    
    void Blueprint::Initialize(Engine* engine, GUID guid) {
        m_Engine = engine;
        m_Guid = guid;
        
        for (size_t i = 0; i < Entries.size(); i++) {
            BlueprintEntry& entry = Entries[i];
            
            switch (entry.TypeID) {
                case EntryType::Entity:
                {
                    m_EntityEntries.push_back(&entry);
                    break;
                }
                case EntryType::TransformData:
                {
                    m_ComponentEntries.push_back(&entry);
                    break;
                }
                case EntryType::ShapeData:
                {
                    m_ComponentEntries.push_back(&entry);
                    break;
                }
                case EntryType::RenderData:
                {
                    m_ComponentEntries.push_back(&entry);
                    break;
                }
                case EntryType::PhysicsData:
                {
                    m_ComponentEntries.push_back(&entry);
                    break;
                }
                case EntryType::Script:
                {
                    m_ScriptEntries.push_back(&entry);
                    break;
                }
                case EntryType::PrefabRef:
                {
                    m_PrefabEntries.push_back(&entry);
                    break;
                }
                case EntryType::Invalid:
                {
                    break;
                }
            }
        }
    }
    
    std::vector<std::shared_ptr<Entity>> Blueprint::Instantiate(std::shared_ptr<Scene> scene) const {
        BlueprintRegistry registry;
        
        /*-----– First Pass -------*/
        
        auto rootEntities = InstantiateEntitiesAndComponents(scene, registry);
        
        /*-----– Second Pass -------*/
        
        BuildHierarchyAndInitializeScripts(scene, registry);
        
        return rootEntities;
    }
    
    nlohmann::json GetProperty(const std::string& propertyName, const BlueprintEntry& entry, const PrefabOverrides& prefabOverrides) {
        if (prefabOverrides.EntryOverrides.find(entry.FileID) == prefabOverrides.EntryOverrides.end())
            return entry.Properties.at(propertyName);
        
        auto& propertyOverrides = prefabOverrides.EntryOverrides.at(entry.FileID);
        if (propertyOverrides.find(propertyName) == propertyOverrides.end())
            return entry.Properties.at(propertyName);
        
        return propertyOverrides.at(propertyName);
    }
    
    std::vector<std::shared_ptr<Entity>> Blueprint::InstantiateEntitiesAndComponents(std::shared_ptr<Scene> scene, BlueprintRegistry& registry, uint64_t instanceID, PrefabOverrides overrides) const {
        std::vector<std::shared_ptr<Entity>> rootEntities;
        RegistryKey registryKey(instanceID, m_Guid);
        
        /* ---- Prefab Entries ---- */
        
        for (size_t i = 0; i < m_PrefabEntries.size(); i++) {
            const BlueprintEntry& entry = *m_PrefabEntries[i];
            
            uint64_t prefabGuid = GetProperty("SourcePrefab", entry, overrides).get<uint64_t>();
            const Blueprint& prefabBlueprint = m_Engine->GetPrefab(prefabGuid).lock()->GetBlueprint();
            
            PrefabOverrides prefabOverrides = entry.Properties.at("Overrides").get<PrefabOverrides>();
            auto prefabRootEntities = prefabBlueprint.InstantiateEntitiesAndComponents(scene, registry, entry.FileID, prefabOverrides);
            
            if (prefabRootEntities.size() != 0)
                rootEntities.push_back(prefabRootEntities[0]);
        }
        
        /* ---- Entity Entries ---- */
        
        auto& entities = registry[registryKey].Entities;
        
        for (size_t i = 0; i < m_EntityEntries.size(); i++) {
            const BlueprintEntry& entry = *m_EntityEntries[i];
            
            if (overrides.RemovedEntities.find(entry.FileID) != overrides.RemovedEntities.end())
                continue;
            
            EntryRef parentRef = GetProperty("Parent", entry, overrides).get<EntryRef>();
            
            auto entity = scene->CreateEntity().lock();
            entities.insert(std::make_pair(entry.FileID, entity));
            entity->SetTag(GetProperty("Tag", entry, overrides).get<std::string>());

            if (parentRef.FileID == 0)
                rootEntities.push_back(entity);
        }
        
        /* ---- Component Entries ---- */
        
        auto& components = registry[registryKey].Components;
        
        for (size_t i = 0; i < m_ComponentEntries.size(); i++) {
            const BlueprintEntry& entry = *m_ComponentEntries[i];
            
            EntryRef parentRef = GetProperty("Parent", entry, overrides).get<EntryRef>();
            
            // If component was attached to an entity which was removed
            if (parentRef.Guid == m_Guid && overrides.RemovedEntities.find(parentRef.InstanceID) != overrides.RemovedEntities.end())
                continue;
            // If component was removed
            if (overrides.RemovedComponents.find(entry.FileID) != overrides.RemovedComponents.end())
                continue;
            
            std::shared_ptr<Entity> parentEntity;
            if (parentRef.FileID == parentRef.InstanceID)
                parentEntity = registry[registryKey].Entities[parentRef.FileID];
            else
                parentEntity = registry[RegistryKey(parentRef.InstanceID, parentRef.Guid)].Entities[parentRef.FileID];
            
            switch (entry.TypeID) {
                case EntryType::TransformData:
                {
                    Vector2 position = GetProperty("Position", entry, overrides).get<Vector2>();
                    Vector2 scale = GetProperty("Scale", entry, overrides).get<Vector2>();
                    
                    auto transform = parentEntity->GetTransformData();
                    transform->SetLocalPosition(position);
                    transform->SetLocalScale(scale);
                    components[entry.FileID] = transform;
                    
                    break;
                }
                case EntryType::ShapeData:
                {
                    Vector2 pivot = GetProperty("Pivot", entry, overrides).get<Vector2>();
                    Vector2 size = GetProperty("Size", entry, overrides).get<Vector2>();
                    
                    auto shapeData = parentEntity->AddShapeData();
                    shapeData->Set(pivot, size);
                    components[entry.FileID] = shapeData;

                    break;
                }
                case EntryType::RenderData:
                {
                    Color tint = GetProperty("Tint", entry, overrides).get<Color>();
                    Rect sourceRect = GetProperty("SourceRect", entry, overrides).get<Rect>();
                    
                    auto renderData = parentEntity->AddRenderData();
                    renderData->Tint = tint;
                    renderData->SourceRect = sourceRect;
                    components[entry.FileID] = renderData;
                    
                    break;
                }
                case EntryType::PhysicsData:
                {
                    bool isStatic = GetProperty("IsStatic", entry, overrides).get<bool>();
                    Vector2 velocity = GetProperty("Velocity", entry, overrides).get<Vector2>();
                    
                    auto physicsData = parentEntity->AddPhysicsData(isStatic).lock();
                    physicsData->Velocity = velocity;
                    components[entry.FileID] = physicsData.get();
                    
                    break;
                }
                default:
                    break;
            }
            
        }
        
        /* ---- Script Entries ---- */
        
        for (size_t i = 0; i < m_ScriptEntries.size(); i++) {
            const BlueprintEntry& entry = *m_ScriptEntries[i];
            
            EntryRef parentRef = GetProperty("Parent", entry, overrides).get<EntryRef>();
            
            // If component was attached to an entity which was removed
            if (parentRef.Guid == m_Guid && overrides.RemovedEntities.find(parentRef.InstanceID) == overrides.RemovedEntities.end())
                continue;
            // If component was removed
            if (overrides.RemovedComponents.find(entry.FileID) == overrides.RemovedComponents.end())
                continue;
            
            //only instantiate and attach to parents
            //add after scripting
        }
        
        return rootEntities;
    }
    
    void Blueprint::BuildHierarchyAndInitializeScripts(std::shared_ptr<Scene> scene, BlueprintRegistry& registries, uint64_t instanceID, PrefabOverrides overrides) const {
        RegistryKey registryKey(instanceID, m_Guid);
        
        for (size_t i = 0; i < m_PrefabEntries.size(); i++) {
            const BlueprintEntry& entry = *m_PrefabEntries[i];
            
            uint64_t prefabGuid = GetProperty("SourcePrefab", entry, overrides).get<uint64_t>();
            const Blueprint& prefabBlueprint = m_Engine->GetPrefab(prefabGuid).lock()->GetBlueprint();
            
            PrefabOverrides prefabOverrides = entry.Properties.at("Overrides").get<PrefabOverrides>();
            prefabBlueprint.BuildHierarchyAndInitializeScripts(scene, registries, entry.FileID, prefabOverrides);
        }
        
        for (size_t i = 0; i < m_EntityEntries.size(); i++) {
            const BlueprintEntry& entry = *m_EntityEntries[i];
            
            EntryRef parentRef = GetProperty("Parent", entry, overrides).get<EntryRef>();
            
            if (overrides.RemovedEntities.find(entry.FileID) != overrides.RemovedEntities.end())
                continue;
        
            auto entity = registries[registryKey].Entities[entry.FileID];
            
            if (parentRef.FileID != 0) {
                std::shared_ptr<Entity> parentEntity;
                if (parentRef.FileID == parentRef.InstanceID)
                    parentEntity = registries[registryKey].Entities[parentRef.FileID];
                else
                    parentEntity = registries[RegistryKey(parentRef.InstanceID, parentRef.Guid)].Entities[parentRef.FileID];
                
                entity->SetParent(parentEntity);
            }
        }
    
        for (size_t i = 0; i < m_ScriptEntries.size(); i++) {
            const BlueprintEntry& entry = *m_ScriptEntries[i];
            
            //add after scripting
        }
    }
}
