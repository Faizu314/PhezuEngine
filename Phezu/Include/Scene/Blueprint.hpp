#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "nlohmann/json.hpp"
#include "Scene/BlueprintEntry.hpp"
#include "Scene/Components/DataComponent.hpp"
#include "Serialization/CustomSerialization.hpp"

namespace Phezu {
    
    class Scene;
    class Engine;
    
    struct RegistryKey {
        uint64_t InstanceID;
        uint64_t PrefabGuid;
        RegistryKey() = default;
        RegistryKey(uint64_t instanceID, uint64_t prefabGuid);
        bool operator==(const RegistryKey& other) const;
        bool operator<(const RegistryKey& other) const;
    };
    
    class Blueprint {
    public:
        Blueprint() = default;
    public:
        Blueprint& operator=(const Blueprint& other) = delete;
    public:
        std::vector<BlueprintEntry> Entries;
    public:
        void Serialize(nlohmann::json& j) const;
        void Deserialize(const nlohmann::json& j);
        //How do we ensure a unique file gets created for a unique list of entities
    private:
        struct Registry {
            std::unordered_map<uint64_t, Entity*> Entities;
            std::unordered_map<uint64_t, DataComponent*> Components;
            Entity* RootEntity;
        };
        using BlueprintRegistry = std::unordered_map<RegistryKey, Registry>;
    public:
        void Initialize(Engine* engine, GUID guid);
        Entity* Instantiate(Scene* scene) const;
    private:
        void InstantiateEntitiesAndComponents(Scene* scene, BlueprintRegistry& registry, uint64_t instanceID = 0, PrefabOverrides overrides = PrefabOverrides()) const;
        void OnEntitiesCreated(BlueprintRegistry& registry) const;
        void OnScriptsInitialized(BlueprintRegistry& registry) const;
        void BuildHierarchyAndInitializeScripts(Scene* scene, BlueprintRegistry& registry, uint64_t instanceID = 0, PrefabOverrides overrides = PrefabOverrides()) const;
    private:
        std::vector<const BlueprintEntry*> m_EntityEntries;
        std::vector<const BlueprintEntry*> m_PrefabEntries;
        std::vector<const BlueprintEntry*> m_ComponentEntries;
        std::vector<const BlueprintEntry*> m_ScriptEntries;
    private:
        Engine* m_Engine;
        GUID m_Guid;
    };
}
