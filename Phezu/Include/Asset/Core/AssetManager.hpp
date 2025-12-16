#pragma once

#include <unordered_map>
#include <vector>
#include <filesystem>
#include "Core/Types/GUID.hpp"
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
        const SceneAsset* GetSceneAsset(GUID guid);
        const PrefabAsset* GetPrefabAsset(GUID guid);
        BuildScenesConfig GetBuildScenesConfig() { return m_BuildScenesConfig; }
    private:
        void LoadAssetMap(const std::filesystem::path& assetsFolder);
        void LoadAssetsInDirectory(const std::filesystem::path& folder);
        void LoadBuildScenesConfig(const std::filesystem::path& buildScenesConfigPath);
    private:
        std::unordered_map<GUID, AssetRef> m_AssetMap;
        std::unordered_map<GUID, void*> m_LoadedAssets;
        BuildScenesConfig m_BuildScenesConfig;
    private:
        Engine* m_Engine;
    };
}
