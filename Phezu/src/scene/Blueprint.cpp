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

    nlohmann::json GetProperty(const std::string& propertyName, const BlueprintEntry& entry, const PrefabOverrides& prefabOverrides) {
        if (prefabOverrides.EntryOverrides.find(entry.FileID) == prefabOverrides.EntryOverrides.end())
            return entry.Properties.at(propertyName);

        auto& propertyOverrides = prefabOverrides.EntryOverrides.at(entry.FileID);
        if (propertyOverrides.find(propertyName) == propertyOverrides.end())
            return entry.Properties.at(propertyName);

        return propertyOverrides.at(propertyName);
    }

    template<typename T>
    T GetProperty(const std::string& propertyName, const BlueprintEntry& entry, const PrefabOverrides& prefabOverrides) {
        return GetProperty(propertyName, entry, prefabOverrides).get<T>();
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
    
    std::shared_ptr<Entity> Blueprint::Instantiate(Scene* scene) const {
        BlueprintRegistry registry;
        
        /*-----– First Pass -------*/
        
        InstantiateEntitiesAndComponents(scene, registry);
        
        /*-----– Second Pass -------*/
        
        BuildHierarchyAndInitializeScripts(scene, registry);

        return registry[RegistryKey(0, m_Guid)].RootEntity;
    }
    
    void Blueprint::InstantiateEntitiesAndComponents(Scene* scene, BlueprintRegistry& registry, uint64_t instanceID, PrefabOverrides overrides) const {
        RegistryKey registryKey(instanceID, m_Guid);
        
        /* ---- Prefab Entries ---- */
        
        for (size_t i = 0; i < m_PrefabEntries.size(); i++) {
            const BlueprintEntry& entry = *m_PrefabEntries[i];
            
            uint64_t prefabGuid = GetProperty<uint64_t>("SourcePrefab", entry, overrides);
            const Blueprint& prefabBlueprint = m_Engine->GetPrefab(prefabGuid).lock()->GetBlueprint();
            
            PrefabOverrides prefabOverrides = entry.Properties.at("Overrides").get<PrefabOverrides>();
            prefabBlueprint.InstantiateEntitiesAndComponents(scene, registry, entry.FileID, prefabOverrides);
        }
        
        /* ---- Entity Entries ---- */
        
        auto& entities = registry[registryKey].Entities;
        
        for (size_t i = 0; i < m_EntityEntries.size(); i++) {
            const BlueprintEntry& entry = *m_EntityEntries[i];
            
            if (overrides.RemovedEntities.find(entry.FileID) != overrides.RemovedEntities.end())
                continue;
            
            EntryRef parentRef = GetProperty<EntryRef>("Parent", entry, overrides);
            
            auto entity = scene->CreateEntity().lock();
            entities.insert(std::make_pair(entry.FileID, entity));
            entity->SetTag(GetProperty<std::string>("Tag", entry, overrides));

            if (parentRef.FileID == 0)
                registry[registryKey].RootEntity = entity;
        }
        
        /* ---- Component Entries ---- */
        
        auto& components = registry[registryKey].Components;
        
        for (size_t i = 0; i < m_ComponentEntries.size(); i++) {
            const BlueprintEntry& entry = *m_ComponentEntries[i];
            
            EntryRef parentRef = GetProperty<EntryRef>("Parent", entry, overrides);
            
            // If component was removed
            if (overrides.RemovedComponents.find(entry.FileID) != overrides.RemovedComponents.end())
                continue;
            // If component was attached to an entity which was removed
            if (parentRef.Guid == m_Guid && overrides.RemovedEntities.find(parentRef.FileID) != overrides.RemovedEntities.end())
                continue;
            
            std::shared_ptr<Entity> parentEntity;
            if (parentRef.Guid == m_Guid)
                parentEntity = registry[registryKey].Entities[parentRef.FileID];
            else
                parentEntity = registry[RegistryKey(parentRef.InstanceID, parentRef.Guid)].Entities[parentRef.FileID];
            
            switch (entry.TypeID) {
                case EntryType::TransformData:
                {
                    Vector2 position = GetProperty<Vector2>("Position", entry, overrides);
                    Vector2 scale = GetProperty<Vector2>("Scale", entry, overrides);
                    
                    auto transform = parentEntity->GetTransformData();
                    transform->SetLocalPosition(position);
                    transform->SetLocalScale(scale);
                    components[entry.FileID] = transform;
                    
                    break;
                }
                case EntryType::ShapeData:
                {
                    Vector2 pivot = GetProperty<Vector2>("Pivot", entry, overrides);
                    Vector2 size = GetProperty<Vector2>("Size", entry, overrides);
                    
                    auto shapeData = parentEntity->AddShapeData();
                    shapeData->Set(pivot, size);
                    components[entry.FileID] = shapeData;

                    break;
                }
                case EntryType::RenderData:
                {
                    Color tint = GetProperty<Color>("Tint", entry, overrides);
                    Rect sourceRect = GetProperty<Rect>("SourceRect", entry, overrides);
                    
                    auto renderData = parentEntity->AddRenderData();
                    renderData->Tint = tint;
                    renderData->SourceRect = sourceRect;
                    components[entry.FileID] = renderData;
                    
                    break;
                }
                case EntryType::PhysicsData:
                {
                    bool isStatic = GetProperty<bool>("IsStatic", entry, overrides);
                    Vector2 velocity = GetProperty<Vector2>("Velocity", entry, overrides);
                    
                    auto physicsData = parentEntity->AddPhysicsData(isStatic);
                    physicsData->Velocity = velocity;
                    components[entry.FileID] = physicsData;
                    
                    break;
                }
                default:
                    break;
            }
            
        }
        
        /* ---- Script Entries ---- */
        
        for (size_t i = 0; i < m_ScriptEntries.size(); i++) {
            const BlueprintEntry& entry = *m_ScriptEntries[i];
            
            EntryRef parentRef = GetProperty<EntryRef>("Parent", entry, overrides);
            
            // If component was attached to an entity which was removed
            if (parentRef.Guid == m_Guid && overrides.RemovedEntities.find(parentRef.InstanceID) != overrides.RemovedEntities.end())
                continue;
            // If component was removed
            if (overrides.RemovedComponents.find(entry.FileID) != overrides.RemovedComponents.end())
                continue;
            
            std::shared_ptr<Entity> parentEntity;
            if (parentRef.Guid == m_Guid)
                parentEntity = registry[registryKey].Entities[parentRef.FileID];
            else
                parentEntity = registry[RegistryKey(parentRef.InstanceID, parentRef.Guid)].Entities[parentRef.FileID];
            
            std::string classFullname = GetProperty<std::string>("Fullname", entry, overrides);

            auto scriptComponent = parentEntity->AddScriptComponent(classFullname);
            components[entry.FileID] = scriptComponent;
        }
    }
    
    void Blueprint::BuildHierarchyAndInitializeScripts(Scene* scene, BlueprintRegistry& registry, uint64_t instanceID, PrefabOverrides overrides) const {
        RegistryKey registryKey(instanceID, m_Guid);
        
        /* ---- Prefab Entries ---- */

        auto& entities = registry[registryKey].Entities;

        for (size_t i = 0; i < m_PrefabEntries.size(); i++) {
            const BlueprintEntry& entry = *m_PrefabEntries[i];
            
            uint64_t prefabGuid = GetProperty<uint64_t>("SourcePrefab", entry, overrides);
            uint64_t parentFileID = GetProperty<uint64_t>("Parent", entry, overrides);
            const Blueprint& prefabBlueprint = m_Engine->GetPrefab(prefabGuid).lock()->GetBlueprint();
            
            PrefabOverrides prefabOverrides = entry.Properties.at("Overrides").get<PrefabOverrides>();
            prefabBlueprint.BuildHierarchyAndInitializeScripts(scene, registry, entry.FileID, prefabOverrides);

            if (parentFileID != 0)
                registry[RegistryKey(entry.FileID, prefabGuid)].RootEntity->SetParent(entities[parentFileID]);
            else
                registry[registryKey].RootEntity = registry[RegistryKey(entry.FileID, prefabGuid)].RootEntity;
        }

        /* ---- Entity Entries ---- */
        
        for (size_t i = 0; i < m_EntityEntries.size(); i++) {
            const BlueprintEntry& entry = *m_EntityEntries[i];  
            
            if (overrides.RemovedEntities.find(entry.FileID) != overrides.RemovedEntities.end())
                continue;
        
            EntryRef parentRef = GetProperty<EntryRef>("Parent", entry, overrides);
            auto entity = entities[entry.FileID];
            
            if (parentRef.FileID != 0) {
                std::shared_ptr<Entity> parentEntity;
                if (parentRef.Guid == m_Guid)
                    parentEntity = entities[parentRef.FileID];
                else
                    parentEntity = registry[RegistryKey(parentRef.InstanceID, parentRef.Guid)].Entities[parentRef.FileID];
                
                entity->SetParent(parentEntity);
            }
        }

        auto& components = registry[registryKey].Components;

        /* ---- Script Entries ---- */
    
        for (size_t i = 0; i < m_ScriptEntries.size(); i++) {
            const BlueprintEntry& entry = *m_ScriptEntries[i];
            
            //add after script field references
        }
    }
}
