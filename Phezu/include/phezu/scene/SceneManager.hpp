#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include "AssetManagement/AssetManager.hpp"

namespace Phezu {
    
    class Engine;
    class Scene;
    
    class SceneManager {
    public:
        SceneManager() = delete;
        SceneManager(Engine* engine);
    public:
        void Init();
        void OnStartGame();
        void OnEndOfFrame();
        void LoadScene(size_t buildIndex);
        void LoadScene(const std::string& sceneName);
        std::weak_ptr<Scene> GetActiveScene() const { return m_ActiveScene; }
        std::weak_ptr<Scene> GetMasterScene() const { return m_MasterScene; }
    public:
        template <typename T>
        void SubscribeToOnSceneLoaded(T* subscriber, void (T::*handler)(void)) {
            if (m_OnSceneLoaded.find(subscriber) != m_OnSceneLoaded.end())
                return;
            
            auto delegate = [subscriber, handler](void) {
                (subscriber->*handler)();
            };
            
            m_OnSceneLoaded.insert(std::make_pair(subscriber, delegate));
        }
        void UnsubscribeToOnSceneLoaded(void* subscriber);
    public:
    private:
        Engine* m_Engine;
    private:
        BuildScenesConfig m_BuildScenesConfig;
        std::shared_ptr<Scene> m_MasterScene;
        std::shared_ptr<Scene> m_ActiveScene;
    private:
        size_t m_SceneToLoad;
        bool m_LoadSceneAfterFrame;
    private:
        std::unordered_map<void*, std::function<void()>> m_OnSceneLoaded;
    };
}
