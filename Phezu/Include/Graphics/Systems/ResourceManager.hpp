// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Core/Defs/Assert.hpp"
#include "Assets/Systems/AssetManager.hpp"

namespace Phezu {

	inline constexpr uint64_t INVALID_RESOURCE_ID = 0;

	enum class ResourceType {
		None = 0,
		Material,
		Mesh,
		Shader,
		Texture
	};

	struct ResourceHandle {
	public:
		ResourceHandle() = default;
		ResourceHandle(uint64_t id, ResourceType type = ResourceType::None) : m_ID(id), m_Type(type) {}
	public:
		uint64_t GetID() const { return m_ID; }
		ResourceType GetType() const { return m_Type; }
	public:
		bool operator==(const ResourceHandle& other) const { return m_ID == other.m_ID; }
		bool operator==(const uint64_t other) const { return m_ID == other; }
	private:
		uint64_t m_ID = INVALID_RESOURCE_ID;
		ResourceType m_Type = ResourceType::None;
	};
}

namespace std {
	template<>
	class hash<Phezu::ResourceHandle> {
	public:
		size_t operator()(const Phezu::ResourceHandle& h) const noexcept {
			return h.GetID();
		}
	};
}

namespace Phezu {

	class IGraphicsAPI;
	class IShader;
	class ITexture;
	class Mesh;
	class Material;

	class ResourceRegistry {
	public:
		ResourceRegistry() : m_ResourceID(1) {}
		void DestroyAndRemoveRecords();
		uint64_t AddRecord(AssetHandle assetHandle, ResourceType type, void* resourcePtr);
		uint64_t AddRecord(ResourceType type, void* resourcePtr);
		void RemoveRecord(void* resourcePtr);
		bool Exists(AssetHandle assetHandle);
		void* GetResource(uint64_t resourceID);
		void* GetResource(AssetHandle assetHandle);
		uint64_t GetResourceID(void* resourcePtr);
		uint64_t GetResourceID(AssetHandle assetHandle);
	private:
		std::unordered_map<void*, ResourceHandle> m_PtrToHandle;
		std::unordered_map<ResourceHandle, void*> m_HandleToPtr;
		std::unordered_map<AssetHandle, ResourceHandle> m_AssetToResource;
		std::unordered_map<ResourceHandle, AssetHandle> m_ResourceToAsset;
	private:
		uint64_t m_ResourceID;
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
		Mesh* GetMesh(AssetHandle meshHandle);
		Material* GetMaterial(AssetHandle materialHandle);
		ITexture* GetTexture(AssetHandle textureHandle);
		IShader* GetShader(AssetHandle shaderHandle);
	public:
		uint64_t CreateUserMaterial(uint64_t sourceMaterialID);
		void DestroyUserMaterial(uint64_t materialID);
		uint64_t GetMaterialID(Material* mat) { return m_Resources.GetResourceID(mat); }
		Material* GetMaterial(uint64_t materialID) { return static_cast<Material*>(m_Resources.GetResource(materialID)); }
	private:
		Mesh* CreateMesh(const MeshAsset* meshAsset);
		Material* CreateMaterial(const MaterialAsset* materialAsset);
		IShader* CreateShader(const ShaderAsset* shaderAsset);
		ITexture* CreateTexture(const TextureAsset* textureAsset);
	private:
		AssetManager* m_AssetManager;
		IGraphicsAPI* m_Api;
	private:
		ResourceRegistry m_Resources;
	};

}