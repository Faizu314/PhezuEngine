#include <filesystem>

#include "Assets/Systems/AssetManager.hpp"
#include "Assets/Core/MetaData.hpp"
#include "Assets/Core/BuiltInAssets.hpp"
#include "Serialization/FileStream.hpp"
#include "Core/Platform.hpp"

namespace Phezu {
    
    static const char* BUILD_SCENE_RELATIVE_PATH = "buildscenes.config";
    
    AssetManager::AssetManager() {}
    
    void AssetManager::Init(const std::filesystem::path& assetsPath) {
        std::filesystem::path buildScenesConfigPath = assetsPath / BUILD_SCENE_RELATIVE_PATH;

        LoadBuiltInAssets();

        PZ_ASSERT(std::filesystem::exists(assetsPath) && std::filesystem::exists(buildScenesConfigPath),
            "Assets or BuildScenesConfig file does not exist.\n");

        LoadAssetMap(assetsPath);
        LoadBuildScenesConfig(buildScenesConfigPath);
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

    void AssetManager::LoadBuiltInAssets() {
        AssetHandle quadMesh(static_cast<int>(BuiltInAssetType::QuadMesh), AssetSource::Engine);
        AssetHandle whiteImage(static_cast<int>(BuiltInAssetType::WhiteImage), AssetSource::Engine);
        AssetHandle defaultTexture(static_cast<int>(BuiltInAssetType::DefaultTexture), AssetSource::Engine);
        AssetHandle spriteShader(static_cast<int>(BuiltInAssetType::SpriteShader), AssetSource::Engine);
        AssetHandle blitShader(static_cast<int>(BuiltInAssetType::BlitShader), AssetSource::Engine);
        AssetHandle spriteMaterial(static_cast<int>(BuiltInAssetType::SpriteMaterial), AssetSource::Engine);
        
        m_LoadedAssets[quadMesh] = BuiltInAssets::CreateQuadMesh();
        m_LoadedAssets[whiteImage] = BuiltInAssets::CreateWhiteImage();
        m_LoadedAssets[defaultTexture] = BuiltInAssets::CreateDefaultTexture();
        m_LoadedAssets[spriteShader] = BuiltInAssets::CreateSpriteShader();
        m_LoadedAssets[blitShader] = BuiltInAssets::CreateBlitShader();
        m_LoadedAssets[spriteMaterial] = BuiltInAssets::CreateSpriteMaterial();
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
        if (m_LoadedAssets.find(assetHandle) == m_LoadedAssets.end())
            return nullptr;

        return m_LoadedAssets[assetHandle];
    }

}
