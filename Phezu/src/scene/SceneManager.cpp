#include "scene/SceneManager.hpp"
#include "scene/Scene.hpp"
#include "Engine.hpp"

namespace Phezu {
    
    SceneManager::SceneManager(Engine* engine) : m_Engine(engine), m_LoadSceneAfterFrame(false), m_ActiveScene(nullptr), m_SceneToLoad(0) {}
    
    void SceneManager::Init() {
        AssetManager& assetManager = m_Engine->GetAssetManager();
        m_BuildScenesConfig = assetManager.GetBuildScenesConfig();
        auto sceneAsset = assetManager.GetSceneAsset(m_BuildScenesConfig.MasterScene);
        m_MasterScene = static_cast<Scene*>(sceneAsset.AssetPtr);
    }

    void SceneManager::OnStartGame() {
        m_MasterScene->Load();
    }
    
    void SceneManager::LoadScene(size_t buildIndex) {
        if (buildIndex < 0 || buildIndex >= m_BuildScenesConfig.BuildScenes.size()) {
            //TODO: assertions
            return;
        }
        
        m_SceneToLoad = buildIndex;
        m_LoadSceneAfterFrame = true;
        
        if (m_ActiveScene)
            m_ActiveScene->BeginUnload();
    }
    
    void SceneManager::LoadScene(const std::string& sceneName) {
        
    }
    
    void SceneManager::UnsubscribeToOnSceneLoaded(void* subscriber) {
        if (m_OnSceneLoaded.find(subscriber) == m_OnSceneLoaded.end())
            return;
        
        m_OnSceneLoaded.erase(subscriber);
    }
    
    void SceneManager::OnEndOfFrame() {
        if (!m_LoadSceneAfterFrame)
            return;
        
        if (m_ActiveScene)
            m_ActiveScene->Unload();
        
        auto sceneAsset = m_Engine->GetAssetManager().GetSceneAsset(m_BuildScenesConfig.BuildScenes[m_SceneToLoad]);
        m_ActiveScene = static_cast<Scene*>(sceneAsset.AssetPtr);
        m_ActiveScene->Load();
        m_LoadSceneAfterFrame = false;
        
        for (auto sub : m_OnSceneLoaded)
            sub.second();
    }
}
