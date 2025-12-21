#pragma once

#include "Asset/Blueprint/Blueprint.hpp"
#include "Scene/Components/DataComponent.hpp"
#include "Serialization/CustomSerialization.hpp"

namespace Phezu {
	
	class AssetManager;
	class ScriptEngine;
	class Scene;
	class Entity;

	struct BlueprintRuntimeContext {
		AssetManager* assetManager;
		ScriptEngine* scriptEngine;
		Scene* scene;
	};

	struct RegistryKey {
		uint64_t InstanceID;
		uint64_t PrefabGuid;
		RegistryKey() = default;
		RegistryKey(uint64_t instanceID, uint64_t prefabGuid);
		bool operator==(const RegistryKey& other) const;
		bool operator<(const RegistryKey& other) const;
	};

	struct Registry {
		std::unordered_map<uint64_t, Entity*> Entities;
		std::unordered_map<uint64_t, DataComponent*> Components;
		Entity* RootEntity;
	};

	using BlueprintRegistry = std::unordered_map<RegistryKey, Registry>;

	class BlueprintInstantiator {
	public:
		static Entity* Instantiate(const BlueprintRuntimeContext& context, const Blueprint& blueprint, GUID bpGuid);
	private:
		static void InstantiateEntitiesAndComponents(const BlueprintRuntimeContext& context, const Blueprint& blueprint, GUID bpGuid, BlueprintRegistry& registry, uint64_t instanceID = 0, PrefabOverrides overrides = PrefabOverrides());
		static void OnEntitiesCreated(const BlueprintRuntimeContext& context, BlueprintRegistry& registry);
		static void BuildHierarchyAndInitializeScripts(const BlueprintRuntimeContext& context, const Blueprint& blueprint, GUID bpGuid, BlueprintRegistry& registry, uint64_t instanceID = 0, PrefabOverrides overrides = PrefabOverrides());
		static void OnScriptsInitialized(const BlueprintRuntimeContext& context, BlueprintRegistry& registry);
	};
}