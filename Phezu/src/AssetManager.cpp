#include "AssetManager.hpp"
#include "scene/Scene.hpp"
#include "scene/Blueprint.hpp"
#include "scene/Prefab.hpp"
#include "serialization/FileStream.hpp"

namespace Phezu {
    
    AssetManager::AssetManager(Engine* engine) : m_Engine(engine) {
        //get all the asset guids from meta files
        //get buildScene guids from config file
    }
    
    Asset AssetManager::GetSceneAsset(size_t buildIndex) {
        if (buildIndex < 0 || buildIndex >= m_BuildScenes.size()) {
            //TODO: assertions
            return Asset();
        }
        
        GUID sceneGuid = m_BuildScenes[buildIndex];
        
        if (m_LoadedAssets.find(sceneGuid) != m_LoadedAssets.end())
            return m_LoadedAssets[sceneGuid];
        
        std::string scenePath = m_AssetMap[sceneGuid].FilePaths[0];
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
        
        std::string prefabPath = m_AssetMap[guid].FilePaths[0];
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
