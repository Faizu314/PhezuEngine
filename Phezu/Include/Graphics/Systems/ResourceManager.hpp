// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

#include "Core/Defs/Assert.hpp"
#include "Assets/Systems/AssetManager.hpp"
#include "Graphics/Systems/ResourceRegistry.hpp"

namespace Phezu {

	class IGraphicsAPI;
	class IShader;
	class ITexture;
	class Mesh;
	class Material;

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
		Mesh* GetMesh(ResourceHandle resourceHandle) { return static_cast<Mesh*>(m_Resources.GetResource(resourceHandle)); }
		Material* GetMaterial(ResourceHandle resourceHandle) { return static_cast<Material*>(m_Resources.GetResource(resourceHandle)); }
		ITexture* GetTexture(ResourceHandle resourceHandle) { return static_cast<ITexture*>(m_Resources.GetResource(resourceHandle)); }
		IShader* GetShader(ResourceHandle resourceHandle) { return static_cast<IShader*>(m_Resources.GetResource(resourceHandle)); }
	public:
		ResourceHandle CreateMaterial(ResourceHandle sourceMaterialHandle);
		void DestroyResource(ResourceHandle resourceHandle);
	public:
		ResourceHandle GetResourceHandle(void* ptr) { return m_Resources.GetResourceHandle(ptr); }
		bool IsResourceHandleValid(ResourceHandle handle, ResourceType type) { return m_Resources.IsResourceHandleValid(handle, type); }
	private:
		Mesh* CreateMeshFromAsset(const MeshAsset* meshAsset);
		Material* CreateMaterialFromAsset(const MaterialAsset* materialAsset);
		IShader* CreateShaderFromAsset(const ShaderAsset* shaderAsset);
		ITexture* CreateTextureFromAsset(const TextureAsset* textureAsset);
	private:
		AssetManager* m_AssetManager;
		IGraphicsAPI* m_Api;
	private:
		ResourceRegistry m_Resources;
	};

}