#include "nlohmann/json.hpp"

#include "Core/Types/Color.hpp"
#include "Asset/Core/AssetManager.hpp"
#include "Asset/Types/PrefabAsset.hpp"
#include "Scene/BlueprintInstantiator.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/ShapeData.hpp"
#include "Scene/Components/RenderData.hpp"
#include "Scene/Components/PhysicsData.hpp"
#include "Scene/Components/ScriptComponent.hpp"
#include "Scripting/ScriptEngine.hpp"

template <>
struct std::hash<Phezu::RegistryKey> {
	std::size_t operator()(const Phezu::RegistryKey& key) const noexcept {
		std::size_t h1 = std::hash<uint64_t>{}(key.InstanceID);
		std::size_t h2 = std::hash<Phezu::AssetHandle>{}(key.PrefabHandle);
		return h1 ^ (h2 << 1);
	}
};

namespace Phezu {
	
	RegistryKey::RegistryKey(uint64_t instanceID, AssetHandle prefabHandle) : InstanceID(instanceID), PrefabHandle(prefabHandle) {}

	bool RegistryKey::operator==(const RegistryKey& other) const {
		return InstanceID == other.InstanceID && PrefabHandle == other.PrefabHandle;
	}

	nlohmann::json GetProperty(const std::string& propertyName, const BlueprintEntry& entry, const PrefabOverrides& prefabOverrides) {
		if (prefabOverrides.EntryOverrides.find(entry.FileID) == prefabOverrides.EntryOverrides.end()) {
			if (entry.Properties.find(propertyName) != entry.Properties.end())
				return entry.Properties.at(propertyName);
			else
				return nlohmann::json();
		}

		auto& propertyOverrides = prefabOverrides.EntryOverrides.at(entry.FileID);
		if (propertyOverrides.find(propertyName) == propertyOverrides.end()) {
			if (entry.Properties.find(propertyName) != entry.Properties.end())
				return entry.Properties.at(propertyName);
			else
				return nlohmann::json();
		}

		return propertyOverrides.at(propertyName);
	}

	template<typename T>
	T GetProperty(const std::string& propertyName, const BlueprintEntry& entry, const PrefabOverrides& prefabOverrides) {
		auto j = GetProperty(propertyName, entry, prefabOverrides);
		return j.is_null() ? T{} : j.get<T>();
	}


	Entity* BlueprintInstantiator::Instantiate(const BlueprintRuntimeContext& context, const Blueprint& blueprint, AssetHandle bpHandle)
	{
		BlueprintRegistry registry;

		/*-----– First Pass -------*/

		InstantiateEntitiesAndComponents(context, blueprint, bpHandle, registry);

		OnEntitiesCreated(context, registry);

		/*-----– Second Pass -------*/

		BuildHierarchyAndInitializeScripts(context, blueprint, bpHandle, registry);

		OnScriptsInitialized(context, registry);

		return registry[RegistryKey(0, bpHandle)].RootEntity;
	}

	void BlueprintInstantiator::InstantiateEntitiesAndComponents(const BlueprintRuntimeContext& context, const Blueprint& bp, AssetHandle bpHandle, BlueprintRegistry& registry, uint64_t instanceID, PrefabOverrides overrides)
	{
        RegistryKey registryKey(instanceID, bpHandle);

        /* ---- Prefab Entries ---- */

        for (size_t i = 0; i < bp.PrefabEntries.size(); i++) {
            const BlueprintEntry& entry = bp.PrefabEntries[i];

            AssetHandle prefabHandle = GetProperty<AssetHandle>("SourcePrefab", entry, overrides);
            auto prefabAsset = context.assetManager->GetAsset<PrefabAsset>(prefabHandle);
            const Blueprint& prefabBlueprint = prefabAsset->GetBlueprint();

            PrefabOverrides prefabOverrides = GetProperty<PrefabOverrides>("Overrides", entry, overrides);
            InstantiateEntitiesAndComponents(context, prefabBlueprint, prefabHandle, registry, entry.FileID, prefabOverrides);
        }

        /* ---- Entity Entries ---- */

        auto& entities = registry[registryKey].Entities;

        for (size_t i = 0; i < bp.EntityEntries.size(); i++) {
            const BlueprintEntry& entry = bp.EntityEntries[i];

            if (overrides.RemovedEntities.find(entry.FileID) != overrides.RemovedEntities.end())
                continue;

            EntryRef parentRef = GetProperty<EntryRef>("Parent", entry, overrides);

            auto entity = context.scene->CreateEntity();
            entities.insert(std::make_pair(entry.FileID, entity));
            entity->Tag = GetProperty<std::string>("Tag", entry, overrides);

            if (parentRef.FileID == 0)
                registry[registryKey].RootEntity = entity;
        }

        /* ---- Component Entries ---- */

        auto& components = registry[registryKey].Components;

        for (size_t i = 0; i < bp.ComponentEntries.size(); i++) {
            const BlueprintEntry& entry = bp.ComponentEntries[i];

            EntryRef parentRef = GetProperty<EntryRef>("Parent", entry, overrides);
            AssetHandle parentHandle(parentRef.Guid, parentRef.Source);

            // If component was removed
            if (overrides.RemovedComponents.find(entry.FileID) != overrides.RemovedComponents.end())
                continue;
            // If component was attached to an entity which was removed
            if (parentHandle == bpHandle && overrides.RemovedEntities.find(parentRef.FileID) != overrides.RemovedEntities.end())
                continue;

            Entity* parentEntity;
            if (parentHandle == bpHandle)
                parentEntity = registry[registryKey].Entities[parentRef.FileID];
            else
                parentEntity = registry[RegistryKey(parentRef.InstanceID, parentHandle)].Entities[parentRef.FileID];

            switch (entry.TypeID) {
                case EntryType::TransformData:
                {
                    Vector2 position = GetProperty<Vector2>("Position", entry, overrides);
                    Vector2 scale = GetProperty<Vector2>("Scale", entry, overrides);

                    auto transform = dynamic_cast<TransformData*>(parentEntity->GetDataComponent(ComponentType::Transform));
                    transform->SetLocalPosition(position);
                    transform->SetLocalScale(scale);
                    components[entry.FileID] = transform;

                    break;
                }
                case EntryType::ShapeData:
                {
                    AssetHandle meshHandle = GetProperty<AssetHandle>("Mesh", entry, overrides);

                    auto shapeData = dynamic_cast<ShapeData*>(parentEntity->AddDataComponent(ComponentType::Shape));
                    shapeData->SetMeshHandle(meshHandle);
                    components[entry.FileID] = shapeData;

                    break;
                }
                case EntryType::RenderData:
                {
                    AssetHandle materialHandle = GetProperty<AssetHandle>("Material", entry, overrides);

                    auto renderData = dynamic_cast<RenderData*>(parentEntity->AddDataComponent(ComponentType::Render));
                    renderData->SetMaterialHandle(materialHandle);
                    components[entry.FileID] = renderData;

                    break;
                }
                case EntryType::PhysicsData:
                {
                    bool isStatic = GetProperty<bool>("IsStatic", entry, overrides);
                    Vector2 velocity = GetProperty<Vector2>("Velocity", entry, overrides);

                    auto physicsData = dynamic_cast<PhysicsData*>(parentEntity->AddDataComponent(ComponentType::Physics));
                    physicsData->Velocity = velocity;
                    physicsData->IsStatic = isStatic;
                    components[entry.FileID] = physicsData;

                    break;
                }
                default:
                    break;
            }

        }

        /* ---- Script Entries ---- */

        for (size_t i = 0; i < bp.ScriptEntries.size(); i++) {
            const BlueprintEntry& entry = bp.ScriptEntries[i];

            EntryRef parentRef = GetProperty<EntryRef>("Parent", entry, overrides);
            AssetHandle parentHandle(parentRef.Guid, parentRef.Source);

            // If component was attached to an entity which was removed
            if (parentHandle == bpHandle && overrides.RemovedEntities.find(parentRef.InstanceID) != overrides.RemovedEntities.end())
                continue;
            // If component was removed
            if (overrides.RemovedComponents.find(entry.FileID) != overrides.RemovedComponents.end())
                continue;

            Entity* parentEntity;
            if (parentHandle == bpHandle)
                parentEntity = registry[registryKey].Entities[parentRef.FileID];
            else
                parentEntity = registry[RegistryKey(parentRef.InstanceID, parentHandle)].Entities[parentRef.FileID];

            std::string classFullname = GetProperty<std::string>("Fullname", entry, overrides);

            ScriptComponent* scriptComponent = parentEntity->AddScriptComponent(classFullname);
            components[entry.FileID] = scriptComponent;
        }

	}

	void BlueprintInstantiator::OnEntitiesCreated(const BlueprintRuntimeContext& context, BlueprintRegistry& registry)
	{
        ScriptEngine& scriptEngine = *context.scriptEngine;

        for (const auto& [registryKey, fileRegistry] : registry) {
            for (const auto& [fileID, entity] : fileRegistry.Entities) {
                scriptEngine.CreateManagedScripts(entity);
            }
        }
	}

	void BlueprintInstantiator::OnScriptsInitialized(const BlueprintRuntimeContext& context, BlueprintRegistry& registry)
	{
        ScriptEngine& scriptEngine = *context.scriptEngine;

        for (const auto& [registryKey, fileRegistry] : registry) {
            for (const auto& [fileID, entity] : fileRegistry.Entities) {
                scriptEngine.InitializeManagedScripts(entity);
            }
        }
	}

	void BlueprintInstantiator::BuildHierarchyAndInitializeScripts(const BlueprintRuntimeContext& context, const Blueprint& bp, AssetHandle bpHandle, BlueprintRegistry& registry, uint64_t instanceID, PrefabOverrides overrides)
	{
        RegistryKey registryKey(instanceID, bpHandle);

        /* ---- Prefab Entries ---- */

        auto& entities = registry[registryKey].Entities;

        for (size_t i = 0; i < bp.PrefabEntries.size(); i++) {
            const BlueprintEntry& entry = bp.PrefabEntries[i];

            AssetHandle prefabHandle = GetProperty<AssetHandle>("SourcePrefab", entry, overrides);
            uint64_t parentFileID = GetProperty<uint64_t>("Parent", entry, overrides);
            auto prefabAsset = context.assetManager->GetAsset<PrefabAsset>(prefabHandle);
            const Blueprint& prefabBlueprint = prefabAsset->GetBlueprint();

            PrefabOverrides prefabOverrides = entry.Properties.at("Overrides").get<PrefabOverrides>();
            BuildHierarchyAndInitializeScripts(context, prefabBlueprint, prefabHandle, registry, entry.FileID, prefabOverrides);

            if (parentFileID != 0)
                registry[RegistryKey(entry.FileID, prefabHandle)].RootEntity->SetParent(entities[parentFileID]);
            else
                registry[registryKey].RootEntity = registry[RegistryKey(entry.FileID, prefabHandle)].RootEntity;
        }

        /* ---- Entity Entries ---- */

        for (size_t i = 0; i < bp.EntityEntries.size(); i++) {
            const BlueprintEntry& entry = bp.EntityEntries[i];

            if (overrides.RemovedEntities.find(entry.FileID) != overrides.RemovedEntities.end())
                continue;

            EntryRef parentRef = GetProperty<EntryRef>("Parent", entry, overrides);
            AssetHandle parentHandle(parentRef.Guid, parentRef.Source);
            auto entity = entities[entry.FileID];

            if (parentRef.FileID != 0) {
                Entity* parentEntity;
                if (parentHandle == bpHandle)
                    parentEntity = entities[parentRef.FileID];
                else
                    parentEntity = registry[RegistryKey(parentRef.InstanceID, parentHandle)].Entities[parentRef.FileID];

                entity->SetParent(parentEntity);
            }
        }

        auto& components = registry[registryKey].Components;

        /* ---- Script Entries ---- */

        for (size_t i = 0; i < bp.ScriptEntries.size(); i++) {
            const BlueprintEntry& entry = bp.ScriptEntries[i];

            std::string classFullname = GetProperty<std::string>("Fullname", entry, overrides);

            uint64_t entityID = components[entry.FileID]->GetEntity()->GetEntityID();

            ScriptOverrides scriptOverrides = GetProperty<ScriptOverrides>("Overrides", entry, overrides);

            ScriptInstance* script = context.scriptEngine->GetBehaviourScriptInstance(entityID, classFullname);

            for (const auto& [name, fieldData] : scriptOverrides) {
                MonoClassField* field = script->GetScriptClass()->GetMonoClassField(name);

                switch (fieldData.Type) {
                    case ScriptFieldType::AssetRef:
                        uint64_t value = fieldData.Value.get<uint64_t>();
                        script->SetAssetRefField(field, value);
                        break;
                }
            }
        }
	}

}