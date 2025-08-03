#include "AssetManagement/AssetManager.hpp"
#include "Engine.hpp"
#include "AssetManagement/MetaData.hpp"
#include "scene/Scene.hpp"
#include "scene/Blueprint.hpp"
#include "scene/Prefab.hpp"
#include "serialization/FileStream.hpp"
#include "Logger.hpp"
#include <filesystem>

namespace Phezu {
    
    static const char* ASSET_RELATIVE_PATH = "Assets";
    static const char* BUILD_SCENE_RELATIVE_PATH = "buildscenes.config";
    
    AssetManager::AssetManager(Engine* engine) : m_Engine(engine) {}
    
    void AssetManager::Init(std::filesystem::path projectPath) {
        std::filesystem::path assetsFolder = projectPath / ASSET_RELATIVE_PATH;
        std::filesystem::path buildScenesConfigPath = projectPath / BUILD_SCENE_RELATIVE_PATH;

        if (std::filesystem::exists(assetsFolder) && std::filesystem::exists(buildScenesConfigPath)) {
            LoadAssetMap(assetsFolder);
            LoadBuildScenesConfig(buildScenesConfigPath);
        }
    }
    
    void AssetManager::LoadAssetMap(const std::filesystem::path& assetsFolder) {
        LoadAssetsInDirectory(assetsFolder);
        
        for (const auto& entry : std::filesystem::directory_iterator(assetsFolder)) {
            if (entry.is_directory()) {
                LoadAssetsInDirectory(entry.path());
            }
        }
    }
    
    void AssetManager::LoadAssetsInDirectory(const std::filesystem::path& folder) {
        for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            if (entry.is_regular_file() && entry.path().extension() == ".meta") {
                FileStreamReader reader(entry.path().string());
                std::string metaDataString;
                metaDataString.resize(reader.Size());
                reader.Read(metaDataString.data(), reader.Size());
                
                MetaData metaData;
                metaData.Deserialize(metaDataString);
                
                std::filesystem::path assetPath = entry.path();
                assetPath.replace_extension("");
                AssetRef& assetRef = m_AssetMap[metaData.Guid];
                assetRef.Filepaths.push_back(assetPath);
            }
        }
    }
    
    void AssetManager::LoadBuildScenesConfig(const std::filesystem::path& buildScenesConfigPath) {
        FileStreamReader reader(buildScenesConfigPath.string());
        std::string configString;
        configString.resize(reader.Size());
        reader.Read(configString.data(), reader.Size());
        
        m_BuildScenesConfig.Deserialize(configString);
    }
    
    Asset AssetManager::GetSceneAsset(GUID guid) {
        if (m_AssetMap.find(guid) == m_AssetMap.end()) {
            Log("Scene Asset with guid: %i not found", guid.Value);
            return Asset();
        }
        if (m_LoadedAssets.find(guid) != m_LoadedAssets.end())
            return m_LoadedAssets[guid];
        
        std::string scenePath = m_AssetMap[guid].Filepaths[0].string();
        std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_Engine);
        
        FileStreamReader reader(scenePath);
        std::string fileContent;
        fileContent.resize(reader.Size());
        reader.Read(fileContent.data(), reader.Size());
        
        scene->Deserialize(fileContent);
        
        Asset sceneAsset;
        sceneAsset.IsLoaded = true;
        sceneAsset.Guid = guid;
        sceneAsset.AssetPtr = std::static_pointer_cast<void>(scene);
        
        m_LoadedAssets[guid] = sceneAsset;
        
        return sceneAsset;
    }
    
    Asset AssetManager::GetPrefabAsset(GUID guid) {
        if (m_AssetMap.find(guid) == m_AssetMap.end()) {
            Log("Prefab Asset with guid: %i not found", guid.Value);
            return Asset();
        }
        if (m_LoadedAssets.find(guid) != m_LoadedAssets.end())
            return m_LoadedAssets[guid];
        
        std::shared_ptr<Prefab> prefab = std::make_shared<Prefab>(m_Engine, guid);
        std::string prefabPath = m_AssetMap[guid].Filepaths[0].string();
        
        FileStreamReader reader(prefabPath);
        std::string fileContent;
        fileContent.resize(reader.Size());
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
