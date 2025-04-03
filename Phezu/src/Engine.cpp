#include <string>

#include "Engine.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "scene/Scene.hpp"
#include "scene/Prefab.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Phezu {
    
    static const size_t ENTITIES_BUFFER_SIZE = 128;

    Engine* Engine::s_Instance = nullptr;
    
    Engine::Engine() : m_HasInited(false), m_IsRunning(false), m_FrameCount(0), m_AssetManager(this), m_SceneManager(this), m_Input(this), m_Physics(this), m_Renderer(nullptr), m_Window(nullptr) {}
    
    int Engine::Init(std::filesystem::path projectPath, const std::string name, int width, int height, int renderScale) {
        if (m_HasInited) {
            //TODO: Logging
            exit(1);
        }

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            //TODO: Logging::Log("Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }

        if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0){
            //TODO: Logging::Log("Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }

        if (TTF_Init() < 0) {
            //TODO: Logging::Log("Failed to init TTF: %s\n", SDL_GetError());
            exit(1);
        }

        mono_set_assemblies_path("mono/lib");

        m_HasInited = true;
        m_ProjectPath = projectPath;
        m_Window = new Window(name, width, height, renderScale);
        m_Renderer = new Renderer(this, *m_Window);
        m_AssetManager.Init(m_ProjectPath);
        m_SceneManager.Init();
        
        return 0;
    }
    
    std::weak_ptr<Scene> Engine::GetMasterScene() {
        return m_SceneManager.GetMasterScene();
    }
    
    void Engine::LoadScene(const std::string& sceneName) {
        if (!m_IsRunning) {
            //TODO: Logging
            return;
        }
        
        m_SceneManager.LoadScene(sceneName);
    }
    
    float GetDeltaTime(Uint64& prevTime, Uint64& freqMs) {
        Uint64 currTime = SDL_GetPerformanceCounter();
        float deltaTime = (currTime - prevTime) / (float)freqMs;
        prevTime = SDL_GetPerformanceCounter();
        
        return deltaTime;
    }
    
    void Engine::Run() {
        if (!m_HasInited || m_Window == nullptr || m_Renderer == nullptr) {
            //TODO: logging file
            return;
        }
        
        m_IsRunning = true;
        
        m_SceneManager.OnStartGame();
        
        Uint64 prevTime = SDL_GetPerformanceCounter();
        Uint64 freqMs = SDL_GetPerformanceFrequency();
        float deltaTime;

        std::vector<std::weak_ptr<Entity>> staticEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        std::vector<std::weak_ptr<Entity>> dynamicEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        std::vector<std::weak_ptr<Entity>> renderEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        size_t renderablesCount;
        size_t staticsCount;
        size_t dynamicsCount;
        
        std::shared_ptr<Scene> masterScene = m_SceneManager.GetMasterScene().lock();
        std::weak_ptr<Scene> activeScene;
        
        while (m_IsRunning)
        {
            if (!m_Input.PollInput())
                break;
            
            renderablesCount = staticsCount = dynamicsCount = 0;

            m_Renderer->ClearFrame();
            
            deltaTime = GetDeltaTime(prevTime, freqMs);
            
            masterScene->LogicUpdate(deltaTime);
            masterScene->GetPhysicsEntities(staticEntitiesBuffer, dynamicEntitiesBuffer, staticsCount, dynamicsCount);
            masterScene->GetRenderableEntities(renderEntitiesBuffer, renderablesCount);
            
            activeScene = m_SceneManager.GetActiveScene();
            
            if (auto sceneL = activeScene.lock()) {
                sceneL->LogicUpdate(deltaTime);
                sceneL->GetPhysicsEntities(staticEntitiesBuffer, dynamicEntitiesBuffer, staticsCount, dynamicsCount);
                sceneL->GetRenderableEntities(renderEntitiesBuffer, renderablesCount);
            }

            m_Physics.PhysicsUpdate(staticEntitiesBuffer, dynamicEntitiesBuffer, staticsCount, dynamicsCount, deltaTime);

            m_Renderer->DrawEntities(renderEntitiesBuffer, renderablesCount);
            
            m_Renderer->RenderFrame();
            
            m_SceneManager.OnEndOfFrame();
            
            m_FrameCount++;
        }
        
        m_IsRunning = false;
        
        Destroy();
    }
    
    void Engine::Destroy() {
        delete m_Renderer;
        delete m_Window;
    }
    
    std::weak_ptr<Prefab> Engine::CreatePrefab() {
        return std::weak_ptr<Prefab>();
    }
    
    std::weak_ptr<Entity> Engine::CreateEntity() {
        auto scene = m_SceneManager.GetActiveScene();
        
        if (auto sceneL = scene.lock())
            return sceneL->CreateEntity();
        else
            return std::weak_ptr<Entity>();
    }
    
    std::weak_ptr<Entity> Engine::CreateEntity(GUID prefabGuid) {
        auto scene = m_SceneManager.GetActiveScene();
        
        if (auto sceneL = scene.lock())
            return sceneL->CreateEntity(prefabGuid);
        else
            return std::weak_ptr<Entity>();
    }
    
    const InputData& Engine::GetInput() {
        return m_Input.GetInput();
    }
    
    SceneManager& Engine::GetSceneManager() {
        return m_SceneManager;
    }
    
    AssetManager& Engine::GetAssetManager() {
        return m_AssetManager;
    }
    
    std::weak_ptr<const Prefab> Engine::GetPrefab(GUID guid) {
        if (!m_HasInited) {
            //TODO: Logging
            return std::weak_ptr<const Prefab>();
        }

        Asset prefabAsset = m_AssetManager.GetPrefabAsset(guid);
        return std::static_pointer_cast<const Prefab>(prefabAsset.AssetPtr);
    }
}
