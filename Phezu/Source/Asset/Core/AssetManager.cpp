#include "Asset/Core/AssetManager.hpp"
#include "Asset/Core/MetaData.hpp"
#include "Asset/Types/SceneAsset.hpp"
#include "Asset/Blueprint/Blueprint.hpp"
#include "Asset/Types/PrefabAsset.hpp"
#include "Serialization/FileStream.hpp"
#include "Core/Platform.hpp"
#include <filesystem>

namespace Phezu {
    
    static const char* BUILD_SCENE_RELATIVE_PATH = "buildscenes.config";
    
    AssetManager::AssetManager(Engine* engine) : m_Engine(engine) {}
    
    void AssetManager::Init(const std::filesystem::path& assetsPath) {
        std::filesystem::path buildScenesConfigPath = assetsPath / BUILD_SCENE_RELATIVE_PATH;

        if (std::filesystem::exists(assetsPath) && std::filesystem::exists(buildScenesConfigPath)) {
            LoadAssetMap(assetsPath);
            LoadBuildScenesConfig(buildScenesConfigPath);
        }
    }
    
    void AssetManager::LoadAssetMap(const std::filesystem::path& assetsFolder) {
        LoadAssetsInDirectory(assetsFolder);
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
            else if (entry.is_directory()) {
                LoadAssetsInDirectory(entry.path());
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
    
    const SceneAsset* AssetManager::GetSceneAsset(GUID guid) {
        if (m_AssetMap.find(guid) == m_AssetMap.end()) {
            Log("Scene Asset with guid: %i not found", guid.Value);
            return nullptr;
        }
        if (m_LoadedAssets.find(guid) != m_LoadedAssets.end())
            return static_cast<SceneAsset*>(m_LoadedAssets[guid]);
        
        std::string scenePath = m_AssetMap[guid].Filepaths[0].string();
        SceneAsset* scene = new SceneAsset();
        
        FileStreamReader reader(scenePath);
        std::string fileContent;
        fileContent.resize(reader.Size());
        reader.Read(fileContent.data(), reader.Size());
        
        scene->Deserialize(fileContent);
        
        m_LoadedAssets[guid] = scene;
        
        return scene;
    }
    
    const PrefabAsset* AssetManager::GetPrefabAsset(GUID guid) {
        if (m_AssetMap.find(guid) == m_AssetMap.end()) {
            Log("Prefab Asset with guid: %i not found", guid.Value);
            return nullptr;
        }
        if (m_LoadedAssets.find(guid) != m_LoadedAssets.end())
            return static_cast<PrefabAsset*>(m_LoadedAssets[guid]);
        
        PrefabAsset* prefab = new PrefabAsset();
        std::string prefabPath = m_AssetMap[guid].Filepaths[0].string();
        
        FileStreamReader reader(prefabPath);
        std::string fileContent;
        fileContent.resize(reader.Size());
        reader.Read(fileContent.data(), reader.Size());
        
        prefab->Deserialize(fileContent);
        
        m_LoadedAssets[guid] = prefab;
        
        return prefab;
    }
}
