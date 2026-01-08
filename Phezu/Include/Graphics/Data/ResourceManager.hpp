#pragma once

#include "Asset/Core/AssetManager.hpp"

namespace Phezu {

	class IGraphicsAPI;
	class IShader;
	class ITexture;
	class Mesh;
	class Material;

	class ResourceManager {
	public:
		void Init(AssetManager* assetManager, IGraphicsAPI* api);
		void Destroy();
	public:
		const Mesh* GetMesh(AssetHandle meshHandle);
		Material* GetMaterial(AssetHandle materialHandle);
		ITexture* GetTexture(AssetHandle textureHandle);
		IShader* GetShader(AssetHandle shaderHandle);
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
	};

}