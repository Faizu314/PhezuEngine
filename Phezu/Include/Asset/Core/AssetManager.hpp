#pragma once

#include <unordered_map>
#include <vector>
#include <filesystem>
#include "Core/Types/GUID.hpp"
#include "Asset/Core/Asset.hpp"
#include "Asset/Configs/BuildScenesConfig.hpp"

namespace Phezu {
    
    class Engine;
    class PrefabAsset;
    class SceneAsset;
    
    struct AssetRef {
        std::vector<std::filesystem::path> Filepaths;
    };
    
    class AssetManager {
    public:
        AssetManager() = delete;
        AssetManager(Engine* engine);
    public:
        void Init(const std::filesystem::path& projectPath);
        template<typename T>
        const T* GetAsset(AssetHandle<T> assetHandle);
        BuildScenesConfig GetBuildScenesConfig() { return m_BuildScenesConfig; }
    private:
        void LoadAssetMap(const std::filesystem::path& assetsFolder);
        void LoadAssetsInDirectory(const std::filesystem::path& folder);
        void LoadBuildScenesConfig(const std::filesystem::path& buildScenesConfigPath);
    private:
        IAsset* TryGetLoadedAsset(GUID guid);
        std::string GetFileFromDisk(const std::filesystem::path& filePath);
    private:
        std::unordered_map<GUID, AssetRef> m_AssetMap;
        std::unordered_map<GUID, IAsset*> m_LoadedAssets;
        BuildScenesConfig m_BuildScenesConfig;
    private:
        Engine* m_Engine;
    };


    template<typename T>
    const T* AssetManager::GetAsset(AssetHandle<T> assetHandle) {
        GUID guid = assetHandle.GetGuid();

        T* asset = static_cast<T*>(TryGetLoadedAsset(guid));
        if (asset != nullptr)
            return asset;

        std::string assetPath = m_AssetMap[guid].Filepaths[0].string();

        asset = new T();
        asset->Deserialize(GetFileFromDisk(assetPath));

        m_LoadedAssets[guid] = asset;

        return asset;
    }
}
