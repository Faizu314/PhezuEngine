#pragma once

#include <unordered_map>
#include <vector>
#include <filesystem>
#include "Core/Types/GUID.hpp"
#include "BuildScenesConfig.hpp"

namespace Phezu {
    
    class Engine;
    
    struct AssetRef {
        std::vector<std::filesystem::path> Filepaths;
    };
    
    struct Asset {
        GUID Guid;
        bool IsLoaded = false;
        void* AssetPtr;
    };
    
    class AssetManager {
    public:
        AssetManager() = delete;
        AssetManager(Engine* engine);
    public:
        void Init(const std::filesystem::path& projectPath);
        Asset GetSceneAsset(GUID guid);
        Asset GetPrefabAsset(GUID guid);
        BuildScenesConfig GetBuildScenesConfig() { return m_BuildScenesConfig; }
    private:
        void LoadAssetMap(const std::filesystem::path& assetsFolder);
        void LoadAssetsInDirectory(const std::filesystem::path& folder);
        void LoadBuildScenesConfig(const std::filesystem::path& buildScenesConfigPath);
    private:
        std::unordered_map<GUID, AssetRef> m_AssetMap;
        std::unordered_map<GUID, Asset> m_LoadedAssets;
        BuildScenesConfig m_BuildScenesConfig;
    private:
        Engine* m_Engine;
    };
}
