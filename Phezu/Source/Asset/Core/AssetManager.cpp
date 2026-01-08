#include <filesystem>

#include "Asset/Core/AssetManager.hpp"
#include "Asset/Core/MetaData.hpp"
#include "Asset/Types/SceneAsset.hpp"
#include "Asset/Types/PrefabAsset.hpp"
#include "Asset/Blueprint/Blueprint.hpp"
#include "Serialization/FileStream.hpp"
#include "Core/Platform.hpp"

namespace Phezu {
    
    static const char* BUILD_SCENE_RELATIVE_PATH = "buildscenes.config";
    
    AssetManager::AssetManager(Engine* engine) : m_Engine(engine) {}
    
    void AssetManager::Init(const std::filesystem::path& assetsPath) {
        std::filesystem::path buildScenesConfigPath = assetsPath / BUILD_SCENE_RELATIVE_PATH;

        if (std::filesystem::exists(assetsPath) && std::filesystem::exists(buildScenesConfigPath)) {
            LoadAssetMap(assetsPath);
            LoadBuildScenesConfig(buildScenesConfigPath);
        }
        else {
            Log("Assert here, Assets or BuildScenesConfig file does not exist\n");
        }
    }

    void AssetManager::Destroy() {
        for (auto& asset : m_LoadedAssets) {
            delete asset.second;
        }
        m_LoadedAssets.clear();
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

                AssetHandle assetHandle(metaData.Guid, AssetSource::Project);
                
                std::filesystem::path assetPath = entry.path();
                assetPath.replace_extension("");
                AssetRef& assetRef = m_AssetMap[assetHandle];
                assetRef.Paths.push_back(assetPath);
                assetRef.Type = metaData.Type;
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

    IAsset* AssetManager::TryGetLoadedAsset(AssetHandle assetHandle) {
        if (m_AssetMap.find(assetHandle) == m_AssetMap.end()) {
            Log("Assert here: Asset with guid: %i not found\n", assetHandle.GetGuid().Value);
            return nullptr;
        }
        if (m_LoadedAssets.find(assetHandle) == m_LoadedAssets.end())
            return nullptr;

        return m_LoadedAssets[assetHandle];
    }

}
