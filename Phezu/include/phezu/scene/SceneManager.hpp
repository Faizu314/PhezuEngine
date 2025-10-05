#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include "AssetManagement/AssetManager.hpp"

namespace Phezu {
    
    class Engine;
    class Scene;
    class Entity;
    
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
        Scene* GetActiveScene() const { return m_ActiveScene; }
        Scene* GetMasterScene() const { return m_MasterScene; }
    public:
        Entity* FindEntity(uint64_t entityID);
    private:
        Engine* m_Engine;
    private:
        BuildScenesConfig m_BuildScenesConfig;
        Scene* m_MasterScene;
        Scene* m_ActiveScene;
    private:
        size_t m_SceneToLoad;
        bool m_LoadSceneAfterFrame;
    };
}
