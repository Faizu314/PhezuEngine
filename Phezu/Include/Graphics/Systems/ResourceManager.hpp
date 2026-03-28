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
		Mesh* GetMesh(uint64_t resourceID) { return static_cast<Mesh*>(m_Resources.GetResource(resourceID)); }
		Material* GetMaterial(uint64_t resourceID) { return static_cast<Material*>(m_Resources.GetResource(resourceID)); }
		ITexture* GetTexture(uint64_t resourceID) { return static_cast<ITexture*>(m_Resources.GetResource(resourceID)); }
		IShader* GetShader(uint64_t resourceID) { return static_cast<IShader*>(m_Resources.GetResource(resourceID)); }
	public:
		uint64_t CreateMaterial(uint64_t sourceMaterialID);
		void DestroyMaterial(uint64_t materialID);
	public:
		uint64_t GetResourceID(Material* mat) { return m_Resources.GetResourceID(mat); }
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