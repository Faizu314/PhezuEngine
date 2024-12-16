#include "scene/SceneManager.hpp"
#include "scene/Scene.hpp"
#include "scene/EntityTemplate.hpp"

namespace Phezu {
    
    SceneManager::SceneManager(Engine* engine) : m_Engine(engine), m_MasterScene(std::make_shared<Scene>(engine, "Master")), m_LoadSceneAfterFrame(false), m_ActiveScene(nullptr) {}
    
    std::weak_ptr<Scene> SceneManager::CreateScene(const std::string& name) {
        std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_Engine, name);
        m_AllScenes.insert(std::make_pair(name, scene));
        return scene;
    }

    void SceneManager::OnStartGame() {
        m_MasterScene->Load();
    }
    
    void SceneManager::LoadScene(const std::string& sceneName) {
        if (m_AllScenes.find(sceneName) == m_AllScenes.end())
            return;
        
        m_SceneToLoad = sceneName;
        m_LoadSceneAfterFrame = true;
        
        if (m_ActiveScene != nullptr)
            m_ActiveScene->BeginUnload();
    }
    
    std::weak_ptr<Scene> SceneManager::GetActiveScene() const {
        if (m_ActiveScene == nullptr)
            return std::weak_ptr<Scene>();
        
        return m_AllScenes.at(m_SceneToLoad);
    }
    
    void SceneManager::UnsubscribeToOnSceneLoaded(void* subscriber) {
        if (m_OnSceneLoaded.find(subscriber) == m_OnSceneLoaded.end())
            return;
        
        m_OnSceneLoaded.erase(subscriber);
    }
    
    void SceneManager::OnEndOfFrame() {
        if (!m_LoadSceneAfterFrame)
            return;
        
        if (m_ActiveScene != nullptr)
            m_ActiveScene->Unload();
        
        m_ActiveScene = m_AllScenes[m_SceneToLoad].get();
        m_ActiveScene->Load();
        m_LoadSceneAfterFrame = false;
        
        for (auto sub : m_OnSceneLoaded)
            sub.second();
    }
}
