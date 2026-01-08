#pragma once

#include "Asset/Core/Asset.hpp"
#include "Asset/Types/SceneAsset.hpp"
#include "Asset/Configs/BuildScenesConfig.hpp"

namespace Phezu {
    
    class Engine;
    class Scene;
    class CameraData;
    
    class SceneManager {
    public:
        SceneManager() = delete;
        SceneManager(Engine* engine);
    public:
        void Init();
        void OnStartGame();
        void OnEndOfFrame();
        void LoadScene(size_t buildIndex);
    public:
        Scene* GetActiveScene() const { return m_ActiveScene; }
        Scene* GetMasterScene() const { return m_MasterScene; }
        CameraData* GetActiveCamera() const { return m_ActiveCamera; }
    public:
        Entity* FindEntity(uint64_t entityID);
    private:
        Scene* LoadScene(AssetHandle sceneGuid);
    private:
        Engine* m_Engine;
    private:
        BuildScenesConfig m_BuildScenesConfig;
        Scene* m_MasterScene;
        Scene* m_ActiveScene;
        CameraData* m_ActiveCamera;
    private:
        size_t m_SceneToLoad;
        bool m_LoadSceneAfterFrame;
    };
}
