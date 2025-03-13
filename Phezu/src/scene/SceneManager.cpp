#include "scene/SceneManager.hpp"
#include "scene/Scene.hpp"
#include "Engine.hpp"

namespace Phezu {
    
    SceneManager::SceneManager(Engine* engine) : m_Engine(engine), m_AssetManager(engine->GetAssetManager()), m_LoadSceneAfterFrame(false), m_ActiveScene(nullptr) {
        m_BuildScenesConfig = m_AssetManager.GetBuildScenesConfig();
        auto sceneAsset = m_AssetManager.GetSceneAsset(m_BuildScenesConfig.MasterScene);
        m_MasterScene = std::static_pointer_cast<Scene>(sceneAsset.AssetPtr.lock());
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
    
    void LoadScene(const std::string& sceneName) {
        
    }
    
    std::weak_ptr<Scene> SceneManager::GetActiveScene() const {
        return m_ActiveScene;
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
        
        auto sceneAsset = m_AssetManager.GetSceneAsset(m_BuildScenesConfig.BuildScenes[m_SceneToLoad]);
        m_ActiveScene = std::static_pointer_cast<Scene>(sceneAsset.AssetPtr.lock());
        m_ActiveScene->Load();
        m_LoadSceneAfterFrame = false;
        
        for (auto sub : m_OnSceneLoaded)
            sub.second();
    }
}
