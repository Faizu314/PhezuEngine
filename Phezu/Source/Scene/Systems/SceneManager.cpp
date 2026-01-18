#include "Core/Engine.hpp"
#include "Assets/Types/SceneAsset.hpp"
#include "Scene/Systems/SceneManager.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/CameraData.hpp"
#include "Scene/Factory/BlueprintInstantiator.hpp"

namespace Phezu {
    
    SceneManager::SceneManager() : m_MasterScene(nullptr), m_ActiveCamera(nullptr), m_LoadSceneAfterFrame(false), m_ActiveScene(nullptr), m_SceneToLoad(0) {}
    
    void SceneManager::Init(SceneContext ctx) {
        m_Ctx = ctx;
        m_BuildScenesConfig = m_Ctx.assetManager->GetBuildScenesConfig();
    }

    void SceneManager::OnStartGame() {
        AssetHandle masterSceneHandle = m_BuildScenesConfig.MasterScene;
        auto sceneAsset = m_Ctx.assetManager->GetAsset<SceneAsset>(masterSceneHandle);
        m_MasterScene = new Scene(m_Ctx, sceneAsset->GetName());
        auto cameraEntity = m_MasterScene->CreateEntity();
        m_ActiveCamera = dynamic_cast<CameraData*>(cameraEntity->AddDataComponent(ComponentType::Camera));
        BlueprintRuntimeContext ctx = { m_Ctx.assetManager, m_Ctx.resourceManager, m_Ctx.scriptEngine, m_MasterScene };
        BlueprintInstantiator::Instantiate(ctx, sceneAsset->GetBlueprint(), masterSceneHandle);
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
    
    Scene* SceneManager::LoadScene(AssetHandle sceneHandle) {
        auto sceneAsset = m_Ctx.assetManager->GetAsset<SceneAsset>(sceneHandle);
        Scene* scene = new Scene(m_Ctx, sceneAsset->GetName());
        BlueprintRuntimeContext ctx = { m_Ctx.assetManager, m_Ctx.resourceManager, m_Ctx.scriptEngine, scene };
        BlueprintInstantiator::Instantiate(ctx, sceneAsset->GetBlueprint(), sceneHandle);

        return scene;
    }
    
    void SceneManager::Update(float deltaTime) {
        m_MasterScene->LogicUpdate(deltaTime);

        if (m_ActiveScene != nullptr) {
            m_ActiveScene->LogicUpdate(deltaTime);
        }
    }

    void SceneManager::GetPhysicsEntities(std::vector<Entity*>& staticEntities, std::vector<Entity*>& dynamicEntities, size_t& staticIndex, size_t& dynamicIndex) {
        m_MasterScene->GetPhysicsEntities(staticEntities, dynamicEntities, staticIndex, dynamicIndex);

        if (m_ActiveScene != nullptr) {
            m_ActiveScene->GetPhysicsEntities(staticEntities, dynamicEntities, staticIndex, dynamicIndex);
        }
    }

    void SceneManager::GetRenderableEntities(std::vector<Entity*>& entities, size_t& index) {
        m_MasterScene->GetRenderableEntities(entities, index);

        if (m_ActiveScene != nullptr) {
            m_ActiveScene->GetRenderableEntities(entities, index);
        }
    }

    void SceneManager::OnEndOfFrame() {
        if (!m_LoadSceneAfterFrame)
            return;
        
        if (m_ActiveScene) {
            m_ActiveScene->Unload();
            delete m_ActiveScene;
        }
        
        m_ActiveScene = LoadScene(m_BuildScenesConfig.BuildScenes[m_SceneToLoad]);
        m_LoadSceneAfterFrame = false;
    }
    
    Entity* SceneManager::FindEntity(uint64_t entityID) {
        Entity* entity = m_MasterScene->GetEntity(entityID);
        
        if (entity != nullptr)
            return entity;
        
        return m_ActiveScene->GetEntity(entityID);
    }
}
