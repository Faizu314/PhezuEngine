#include "scene/Blueprint.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "nlohmann/json.hpp"
#include "scene/Prefab.hpp"
#include "Engine.hpp"

namespace Phezu {
    
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
        
        auto& propertyOverrides = prefabOverrides.EntryOverrides.at(entry.FileID).PropertyOverrides;
        if (propertyOverrides.find(propertyName) == propertyOverrides.end())
            return entry.Properties.at(propertyName);
        
        return propertyOverrides.at(propertyName);
    }
    
    std::vector<std::shared_ptr<Entity>> Blueprint::InstantiateEntitiesAndComponents(std::shared_ptr<Scene> scene, BlueprintRegistry& registry, PrefabOverrides overrides) const {
        std::vector<std::shared_ptr<Entity>> rootEntities;
        
        /* ---- Prefab Entries ---- */
        
        for (size_t i = 0; i < m_PrefabEntries.size(); i++) {
            const BlueprintEntry& entry = *m_PrefabEntries[i];
            
            uint64_t prefabGuid = GetProperty("SourcePrefab", entry, overrides).get<uint64_t>();
            const Blueprint& prefabBlueprint = m_Engine->GetPrefab(prefabGuid).lock()->GetBlueprint();
            
            PrefabOverrides prefabOverrides = entry.Properties.at("Overrides").get<PrefabOverrides>();
            auto prefabRootEntities = prefabBlueprint.InstantiateEntitiesAndComponents(scene, registry, prefabOverrides);
            
            if (prefabRootEntities.size() != 0)
                rootEntities.push_back(prefabRootEntities[0]);
        }
        
        /* ---- Entity Entries ---- */
        
        auto& entities = registry.Files[m_Guid].Entities;
        
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
        
        auto& components = registry.Files[m_Guid].Components;
        
        for (size_t i = 0; i < m_ComponentEntries.size(); i++) {
            const BlueprintEntry& entry = *m_ComponentEntries[i];
            
            EntryRef parentRef = GetProperty("Parent", entry, overrides).get<EntryRef>();
            
            // If component was attached to an entity which was removed
            if (parentRef.Guid == m_Guid && overrides.RemovedEntities.find(parentRef.FileID) != overrides.RemovedEntities.end())
                continue;
            // If component was removed
            if (overrides.RemovedComponents.find(entry.FileID) != overrides.RemovedComponents.end())
                continue;
            
            switch (entry.TypeID) {
                case EntryType::TransformData:
                {
                    Vector2 position = GetProperty("Position", entry, overrides).get<Vector2>();
                    Vector2 scale = GetProperty("Scale", entry, overrides).get<Vector2>();
                    
                    auto transform = registry.Files[m_Guid].Entities[parentRef.FileID]->GetTransformData();
                    transform->SetLocalPosition(position);
                    transform->SetLocalScale(scale);
                    components[entry.FileID] = transform;
                    
                    break;
                }
                case EntryType::ShapeData:
                {
                    Vector2 pivot = GetProperty("Pivot", entry, overrides).get<Vector2>();
                    Vector2 size = GetProperty("Size", entry, overrides).get<Vector2>();
                    
                    auto shapeData = registry.Files[parentRef.Guid].Entities[parentRef.FileID]->AddShapeData();
                    shapeData->Set(pivot, size);
                    components[entry.FileID] = shapeData;

                    break;
                }
                case EntryType::RenderData:
                {
                    Color tint = GetProperty("Tint", entry, overrides).get<Color>();
                    Rect sourceRect = GetProperty("SourceRect", entry, overrides).get<Rect>();
                    
                    auto renderData = registry.Files[parentRef.Guid].Entities[parentRef.FileID]->AddRenderData();
                    renderData->Tint = tint;
                    renderData->SourceRect = sourceRect;
                    components[entry.FileID] = renderData;
                    
                    break;
                }
                case EntryType::PhysicsData:
                {
                    bool isStatic = GetProperty("IsStatic", entry, overrides).get<bool>();
                    Vector2 velocity = GetProperty("Velocity", entry, overrides).get<Vector2>();
                    
                    auto physicsData = registry.Files[parentRef.Guid].Entities[parentRef.FileID]->AddPhysicsData(isStatic).lock();
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
            if (parentRef.Guid == m_Guid && overrides.RemovedEntities.find(parentRef.FileID) == overrides.RemovedEntities.end())
                continue;
            // If component was removed
            if (overrides.RemovedComponents.find(entry.FileID) == overrides.RemovedComponents.end())
                continue;
            
            //only instantiate and attach to parents
            //add after scripting
        }
        
        return rootEntities;
    }
    
    void Blueprint::BuildHierarchyAndInitializeScripts(std::shared_ptr<Scene> scene, BlueprintRegistry& registry, PrefabOverrides overrides) const {
        
        for (size_t i = 0; i < m_PrefabEntries.size(); i++) {
            const BlueprintEntry& entry = *m_PrefabEntries[i];
            
            uint64_t prefabGuid = GetProperty("SourcePrefab", entry, overrides).get<uint64_t>();
            const Blueprint& prefabBlueprint = m_Engine->GetPrefab(prefabGuid).lock()->GetBlueprint();
            
            PrefabOverrides prefabOverrides = entry.Properties.at("Overrides").get<PrefabOverrides>();
            prefabBlueprint.BuildHierarchyAndInitializeScripts(scene, registry, prefabOverrides);
        }
        
        for (size_t i = 0; i < m_EntityEntries.size(); i++) {
            const BlueprintEntry& entry = *m_EntityEntries[i];
            
            EntryRef parentRef = GetProperty("Parent", entry, overrides).get<EntryRef>();
            
            if (overrides.RemovedEntities.find(entry.FileID) != overrides.RemovedEntities.end())
                continue;
        
            auto entity = registry.Files[m_Guid].Entities[entry.FileID];
            
            if (parentRef.FileID != 0)
                entity->SetParent(registry.Files[parentRef.Guid].Entities[parentRef.FileID]);
        }
    
        for (size_t i = 0; i < m_ScriptEntries.size(); i++) {
            const BlueprintEntry& entry = *m_ScriptEntries[i];
            
            //add after scripting
        }
    }
}
