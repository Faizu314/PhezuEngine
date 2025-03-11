#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "GUID.hpp"

namespace Phezu {
    
    class Engine;
    
    struct AssetRef {
        std::vector<std::string> FilePaths;
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
        std::unordered_map<GUID, AssetRef> m_AssetMap;
        std::unordered_map<GUID, Asset> m_LoadedAssets;
        std::vector<GUID> m_BuildScenes;
    private:
        Engine* m_Engine;
    };
}
