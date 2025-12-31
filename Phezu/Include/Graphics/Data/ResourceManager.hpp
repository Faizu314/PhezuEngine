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
		const Mesh* GetMesh(AssetHandle<MeshAsset> meshHandle);
		Material* GetMaterial(AssetHandle<MaterialAsset> materialHandle);
		ITexture* GetTexture(AssetHandle<TextureAsset> textureHandle);
		IShader* GetShader(AssetHandle<ShaderAsset> shaderHandle);
	private:
		Mesh* CreateMesh(const MeshAsset* meshAsset);
		Material* CreateMaterial(const MaterialAsset* materialAsset);
		IShader* CreateShader(const ShaderAsset* shaderAsset);
		ITexture* CreateTexture(const TextureAsset* textureAsset);
	private:
		AssetManager* m_AssetManager;
		IGraphicsAPI* m_Api;
	private:
		std::unordered_map<GUID, Mesh*> m_Meshes;
		std::unordered_map<GUID, Material*> m_Materials;
		std::unordered_map<GUID, IShader*> m_Shaders;
		std::unordered_map<GUID, ITexture*> m_Textures;
	};

}