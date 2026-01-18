#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Core/Platform.hpp"
#include "Assets/Systems/AssetManager.hpp"

namespace Phezu {

	class IGraphicsAPI;
	class IShader;
	class ITexture;
	class Mesh;
	class Material;

	template<typename T>
	struct ResourceRegistry {
		std::unordered_map<uint64_t, T*> m_IdToResource;
		std::unordered_map<T*, uint64_t> m_ResourceToId;
		uint64_t m_ResourceID;

		ResourceRegistry() : m_ResourceID(1) { }

		uint64_t AddRecord(T* resourcePtr) {
			while (m_IdToResource.find(m_ResourceID) != m_IdToResource.end() || m_ResourceID == 0)
				m_ResourceID++;

			m_IdToResource.insert(std::pair(m_ResourceID, resourcePtr));
			m_ResourceToId.insert(std::pair(resourcePtr, m_ResourceID));

			return m_ResourceID;
		}

		void RemoveRecord(T* resourcePtr) {
			if (m_ResourceToId.find(resourcePtr) == m_ResourceToId.end())
				return;

			uint64_t materialID = m_ResourceToId.at(resourcePtr);
			m_ResourceToId.erase(resourcePtr);
			m_IdToResource.erase(materialID);
		}

		T* GetResource(uint64_t resourceID) {
			if (m_IdToResource.find(resourceID) == m_IdToResource.end()) {
				Log("Assert here\n");
				return nullptr;
			}

			return m_IdToResource.at(resourceID);
		}

		uint64_t GetResourceID(T* resourcePtr) {
			if (m_ResourceToId.find(resourcePtr) == m_ResourceToId.end()) {
				Log("Assert here\n");
				return 0;
			}

			return m_ResourceToId.at(resourcePtr);
		}
	};

	class ResourceManager {
	public:
		ResourceManager() = default;
	public:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(const ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&&) = delete;
	public:
		void Init(AssetManager* assetManager, IGraphicsAPI* api);
		void Destroy();
	public:
		const Mesh* GetMesh(AssetHandle meshHandle);
		Material* GetMaterial(AssetHandle materialHandle);
		ITexture* GetTexture(AssetHandle textureHandle);
		IShader* GetShader(AssetHandle shaderHandle);
	public:
		bool IsUserMaterial(uint64_t materialID);
		uint64_t CreateUserMaterial(uint64_t sourceMaterialID);
		void DestroyUserMaterial(uint64_t materialID);
		uint64_t GetMaterialID(Material* mat) { return m_MaterialRegistry.GetResourceID(mat); }
		Material* GetMaterial(uint64_t materialID) { return m_MaterialRegistry.GetResource(materialID); }
	private:
		Mesh* CreateMesh(const MeshAsset* meshAsset);
		Material* CreateMaterial(const MaterialAsset* materialAsset);
		IShader* CreateShader(const ShaderAsset* shaderAsset);
		ITexture* CreateTexture(const TextureAsset* textureAsset);
	private:
		AssetManager* m_AssetManager;
		IGraphicsAPI* m_Api;
	private:
		std::unordered_map<AssetHandle, Mesh*> m_Meshes;
		std::unordered_map<AssetHandle, Material*> m_Materials;
		std::unordered_map<AssetHandle, IShader*> m_Shaders;
		std::unordered_map<AssetHandle, ITexture*> m_Textures;
	private:
		std::unordered_set<Material*> m_UserMaterials;
		ResourceRegistry<Material> m_MaterialRegistry;
	};

}