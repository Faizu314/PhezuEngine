#pragma once

#include <unordered_map>
#include <vector>
#include <filesystem>

#include "Core/Platform.hpp"
#include "Core/Types/GUID.hpp"
#include "Asset/Core/Asset.hpp"
#include "Asset/Types/ImageAsset.hpp"
#include "Asset/Types/TextureAsset.hpp"
#include "Asset/Types/MaterialAsset.hpp"
#include "Asset/Types/MeshAsset.hpp"
#include "Asset/Types/ShaderAsset.hpp"
#include "Asset/Types/PrefabAsset.hpp"
#include "Asset/Types/SceneAsset.hpp"
#include "Asset/Configs/BuildScenesConfig.hpp"

namespace Phezu {
    
    class Engine;
    class PrefabAsset;
    class SceneAsset;
    
    using AssetPaths = std::vector<std::filesystem::path>;

    struct AssetRef {
        std::vector<std::filesystem::path> Paths;
        AssetType Type;
    };
    
    class AssetManager {
    public:
        AssetManager() = delete;
        AssetManager(Engine* engine);
    public:
        void Init(const std::filesystem::path& projectPath);
        void Destroy();
    public:
        template<typename T>
        const T* GetAsset(AssetHandle assetHandle);
        BuildScenesConfig GetBuildScenesConfig() { return m_BuildScenesConfig; }
    private:
        void LoadAssetMap(const std::filesystem::path& assetsFolder);
        void LoadAssetsInDirectory(const std::filesystem::path& folder);
        void LoadBuiltInAssets();
        void LoadBuildScenesConfig(const std::filesystem::path& buildScenesConfigPath);
    private:
        IAsset* TryGetLoadedAsset(AssetHandle guid);
        std::string GetFileFromDisk(const std::filesystem::path& filePath);
    private:
        std::unordered_map<AssetHandle, AssetRef> m_AssetMap;
        std::unordered_map<AssetHandle, IAsset*> m_LoadedAssets;
        BuildScenesConfig m_BuildScenesConfig;
    private:
        Engine* m_Engine;
    };


    template<typename T>
    const T* AssetManager::GetAsset(AssetHandle assetHandle) {
        static_assert(std::is_base_of_v<IAsset, T>, "T must derive from IAsset");

        GUID guid = assetHandle.GetGuid();
        AssetSource source = assetHandle.GetSource();

        IAsset* iAsset = TryGetLoadedAsset(assetHandle);
        T* asset = nullptr;

        if (iAsset != nullptr) {
            asset = dynamic_cast<T*>(iAsset);

            if (asset == nullptr) {
                Log("Should assert here, asset guid does not match its type.\n");
                return nullptr;
            }
            if (asset != nullptr)
                return asset;
        }

        AssetRef& ref = m_AssetMap[assetHandle];

        asset = new T();

        if (ref.Type != asset->GetAssetType()) {
            Log("Should assert here, asset guid does not match its type.\n");
            return nullptr;
        }

        std::string assetPath = ref.Paths[0].string();
        asset->Deserialize(GetFileFromDisk(assetPath));

        m_LoadedAssets[assetHandle] = asset;

        return asset;
    }
}
