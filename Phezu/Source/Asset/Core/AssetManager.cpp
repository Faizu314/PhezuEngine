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

    std::string AssetManager::GetFileFromDisk(const std::filesystem::path& filePath) {
        FileStreamReader reader(filePath.string());
        std::string fileContent;
        fileContent.resize(reader.Size());
        reader.Read(fileContent.data(), reader.Size());

        return fileContent;
    }

    IAsset* AssetManager::TryGetLoadedAsset(GUID guid) {
        if (m_AssetMap.find(guid) == m_AssetMap.end()) {
            Log("Scene Asset with guid: %i not found\n", guid.Value);
            return nullptr;
        }
        if (m_LoadedAssets.find(guid) == m_LoadedAssets.end())
            return nullptr;

        return m_LoadedAssets[guid];
    }

}
