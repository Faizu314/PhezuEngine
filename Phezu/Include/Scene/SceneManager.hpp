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
        void Update(float deltaTime);
        void OnEndOfFrame();
        void LoadScene(size_t buildIndex);
    public:
        Scene* GetActiveScene() const { return m_ActiveScene; }
        Scene* GetMasterScene() const { return m_MasterScene; }
        CameraData* GetActiveCamera() const { return m_ActiveCamera; }
    public:
        void GetPhysicsEntities(std::vector<Entity*>& staticEntities, std::vector<Entity*>& dynamicEntities, size_t& staticIndex, size_t& dynamicIndex);
        void GetRenderableEntities(std::vector<Entity*>& entities, size_t& index);
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
