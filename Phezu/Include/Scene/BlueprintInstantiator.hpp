#pragma once

#include "Assets/Core/Asset.hpp"
#include "Assets/Blueprint/Blueprint.hpp"
#include "Scene/Components/DataComponent.hpp"
#include "Serialization/CustomSerialization.hpp"

namespace Phezu {
	
	class AssetManager;
	class ResourceManager;
	class ScriptEngine;
	class Scene;
	class Entity;

	struct BlueprintRuntimeContext {
		AssetManager* assetManager;
		ResourceManager* resourceManager;
		ScriptEngine* scriptEngine;
		Scene* scene;
	};

	struct RegistryKey {
		RegistryKey() = default;
		RegistryKey(uint64_t instanceID, AssetHandle prefabHandle);
		bool operator==(const RegistryKey& other) const;
		uint64_t InstanceID;
		AssetHandle PrefabHandle;
	};

	struct BlueprintInstance {
		std::unordered_map<uint64_t, Entity*> Entities;
		std::unordered_map<uint64_t, DataComponent*> Components;
		Entity* RootEntity;
	};

	using BlueprintRegistry = std::unordered_map<RegistryKey, BlueprintInstance>;

	class BlueprintInstantiator {
	public:
		static Entity* Instantiate(const BlueprintRuntimeContext& context, const Blueprint& blueprint, AssetHandle bpHandle);
	private:
		static void InstantiateEntitiesAndComponents(const BlueprintRuntimeContext& context, const Blueprint& blueprint, AssetHandle bpHandle, BlueprintRegistry& registry, uint64_t instanceID = 0, PrefabOverrides overrides = PrefabOverrides());
		static void OnEntitiesCreated(const BlueprintRuntimeContext& context, BlueprintRegistry& registry);
		static void BuildHierarchyAndInitializeScripts(const BlueprintRuntimeContext& context, const Blueprint& blueprint, AssetHandle bpHandle, BlueprintRegistry& registry, uint64_t instanceID = 0, PrefabOverrides overrides = PrefabOverrides());
		static void OnScriptsInitialized(const BlueprintRuntimeContext& context, BlueprintRegistry& registry);
	};
}