#include "Scene/SceneManager.hpp"
#include "Core/Engine.hpp"
#include "Asset/Types/SceneAsset.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/CameraData.hpp"
#include "Scene/BlueprintInstantiator.hpp"

namespace Phezu {
    
    SceneManager::SceneManager(Engine* engine) : m_Engine(engine), m_MasterScene(nullptr), m_ActiveCamera(nullptr), m_LoadSceneAfterFrame(false), m_ActiveScene(nullptr), m_SceneToLoad(0) {}
    
    void SceneManager::Init() {
        AssetManager& assetManager = m_Engine->GetAssetManager();
        m_BuildScenesConfig = assetManager.GetBuildScenesConfig();

        m_MasterScene = new Scene(m_Engine, "MasterScene");
    }

    void SceneManager::OnStartGame() {
        LoadScene(AssetHandle<SceneAsset>{ m_BuildScenesConfig.MasterScene });

        auto cameraEntity = m_MasterScene->CreateEntity();
        m_ActiveCamera = dynamic_cast<CameraData*>(cameraEntity->AddDataComponent(ComponentType::Camera));
    }
    
    void SceneManager::LoadScene(size_t buildIndex) {
        if (buildIndex < 0 || buildIndex >= m_BuildScenesConfig.BuildScenes.size()) {
            Log("Assert here\n");
            return;
        }
        
        m_SceneToLoad = buildIndex;
        m_LoadSceneAfterFrame = true;
        
        if (m_ActiveScene)
            m_ActiveScene->BeginUnload();
    }
    
    Scene* SceneManager::LoadScene(AssetHandle<SceneAsset> sceneHandle) {
        const SceneAsset* masterSceneAsset = m_Engine->GetAssetManager().GetAsset(sceneHandle);
        Scene* scene = new Scene(m_Engine, masterSceneAsset->GetName());
        BlueprintRuntimeContext ctx = { &m_Engine->GetAssetManager(), &m_Engine->GetScriptEngine(), scene };
        BlueprintInstantiator::Instantiate(ctx, masterSceneAsset->GetBlueprint());

        return scene;
    }
    
    void SceneManager::OnEndOfFrame() {
        if (!m_LoadSceneAfterFrame)
            return;
        
        if (m_ActiveScene) {
            m_ActiveScene->Unload();
            delete m_ActiveScene;
        }
        
        m_ActiveScene = LoadScene(AssetHandle<SceneAsset>{ m_BuildScenesConfig.BuildScenes[m_SceneToLoad] });
        m_LoadSceneAfterFrame = false;
    }
    
    Entity* SceneManager::FindEntity(uint64_t entityID) {
        Entity* entity = m_MasterScene->GetEntity(entityID);
        
        if (entity != nullptr)
            return entity;
        
        return m_ActiveScene->GetEntity(entityID);
    }
}
