#include "AssetManagement/AssetManager.hpp"
#include "AssetManagement/MetaData.hpp"
#include "scene/Scene.hpp"
#include "scene/Blueprint.hpp"
#include "scene/Prefab.hpp"
#include "serialization/FileStream.hpp"
#include <filesystem>

namespace Phezu {
    
    AssetManager::AssetManager(Engine* engine) : m_Engine(engine) {
        std::filesystem::path assetsFolder;
        std::filesystem::path buildScenesConfigPath;

        ConstructAssetMap(assetsFolder);
        GetBuildSceneGuids(buildScenesConfigPath);
    }
    
    void AssetManager::ConstructAssetMap(const std::filesystem::path& assetsFolder) {
        for (const auto& entry : std::filesystem::directory_iterator(assetsFolder)) {
            if (entry.is_regular_file() && entry.path().extension() == ".meta") {
                FileStreamReader reader(entry.path());
                std::string metaDataString;
                metaDataString.reserve(reader.Size());
                reader.Read(metaDataString.data(), reader.Size());
                
                MetaData metaData;
                metaData.Deserialize(metaDataString);
                
                AssetRef& assetRef = m_AssetMap[metaData.Guid];
                assetRef.Filepaths.push_back(entry.path());
            }
        }
    }
    
    void AssetManager::GetBuildSceneGuids(const std::filesystem::path& buildScenesConfigPath) {
        FileStreamReader reader(buildScenesConfigPath.string());
        std::string configString;
        configString.reserve(reader.Size());
        reader.Read(configString.data(), reader.Size());
        
        m_BuildScenesConfig.Deserialize(configString);
    }
    
    Asset AssetManager::GetSceneAsset(size_t buildIndex) {
        auto& buildScenes = m_BuildScenesConfig.BuildScenes;
        
        if (buildIndex < 0 || buildIndex >= buildScenes.size()) {
            //TODO: assertions
            return Asset();
        }
        
        GUID sceneGuid = buildScenes[buildIndex];
        
        if (m_LoadedAssets.find(sceneGuid) != m_LoadedAssets.end())
            return m_LoadedAssets[sceneGuid];
        
        std::string scenePath = m_AssetMap[sceneGuid].Filepaths[0];
        std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_Engine);
        
        FileStreamReader reader(scenePath);
        std::string fileContent;
        reader.Read(fileContent.data(), reader.Size());
        
        scene->Deserialize(fileContent);
        
        Asset sceneAsset;
        sceneAsset.IsLoaded = true;
        sceneAsset.Guid = sceneGuid;
        sceneAsset.AssetPtr = std::static_pointer_cast<void>(scene);
        
        m_LoadedAssets[sceneGuid] = sceneAsset;
        
        return sceneAsset;
    }
    
    Asset AssetManager::GetPrefabAsset(GUID guid) {
        if (m_AssetMap.find(guid) == m_AssetMap.end()) {
            //TODO: assert
            return Asset();
        }
        if (m_LoadedAssets.find(guid) != m_LoadedAssets.end())
            return m_LoadedAssets[guid];
        
        std::string prefabPath = m_AssetMap[guid].Filepaths[0];
        std::shared_ptr<Prefab> prefab = std::make_shared<Prefab>(guid);
        
        FileStreamReader reader(prefabPath);
        std::string fileContent;
        reader.Read(fileContent.data(), reader.Size());
        
        prefab->Deserialize(fileContent);
        
        Asset prefabAsset;
        prefabAsset.IsLoaded = true;
        prefabAsset.Guid = guid;
        prefabAsset.AssetPtr = std::static_pointer_cast<void>(prefab);
        
        m_LoadedAssets[guid] = prefabAsset;
        
        return prefabAsset;
    }
}
