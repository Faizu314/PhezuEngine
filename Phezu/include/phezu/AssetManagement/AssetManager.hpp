#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include "GUID.hpp"
#include "BuildScenesConfig.hpp"

namespace Phezu {
    
    class Engine;
    
    struct AssetRef {
        std::vector<std::filesystem::path> Filepaths;
    };
    
    struct Asset {
        GUID Guid;
        bool IsLoaded = false;
        std::weak_ptr<void> AssetPtr;
    };
    
    class AssetManager {
    public:
        AssetManager() = delete;
        AssetManager(Engine* engine);
    public:
        Asset GetSceneAsset(size_t buildIndex);
        Asset GetPrefabAsset(GUID guid);
    private:
        void ConstructAssetMap(const std::filesystem::path& assetsFolder);
        void GetBuildSceneGuids(const std::filesystem::path& buildScenesConfigPath);
    private:
        std::unordered_map<GUID, AssetRef> m_AssetMap;
        std::unordered_map<GUID, Asset> m_LoadedAssets;
        BuildScenesConfig m_BuildScenesConfig;
    private:
        Engine* m_Engine;
    };
}
