#include <string>

#include "Engine.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Prefab.hpp"
#include "Logger.hpp"

namespace Phezu {
    
    static const float MAX_DELTA_TIME = 1.0f / 20.0f;
    static const float TARGET_FRAME_TIME = 1.0f / 60.0f;
    static const size_t ENTITIES_BUFFER_SIZE = 128;

    Engine* Engine::s_Instance = nullptr;
    
    Engine::Engine() : m_HasInited(false), m_IsRunning(false),
        m_FrameCount(0), m_AssetManager(this), m_SceneManager(this),
        m_Physics(this), m_Renderer(nullptr),
        m_Window(nullptr), m_ScriptEngine(this) {}
    
    int Engine::Init(EngineArgs& config) {
        if (m_HasInited) {
            Log("Trying to init engine twice");
            return 1;
        }

        InitLogger();

        m_HasInited = true;
        m_ExePath = config.AllPaths.ExePath;
        m_AssetsPath = config.AllPaths.AssetsPath;
        m_ScriptCoreDllPath = config.AllPaths.ScriptCoreDllPath;
        m_MonoCoreLibsPath = config.AllPaths.MonoCoreLibsPath;
        m_Window = new Window(config.Name, config.ResolutionSettings.Width, config.ResolutionSettings.Height, config.ResolutionSettings.RenderScale);
        m_Renderer = new Renderer(this, *m_Window);
        m_AssetManager.Init(m_AssetsPath);
        m_SceneManager.Init();
        m_ScriptEngine.Init();
        Input::Init();
        
        return 0;
    }
    
    void Engine::Run() {
        if (!m_HasInited || m_Window == nullptr || m_Renderer == nullptr) {
            Log("Trying to run non inited engine");
            return;
        }
        
        m_IsRunning = true;
        
        m_Renderer->SetActiveCamera(m_SceneManager.GetActiveCamera());
        m_SceneManager.OnStartGame();
        
        //Uint64 frameStartTime = SDL_GetPerformanceCounter();
        //Uint64 freqMs = SDL_GetPerformanceFrequency();
        float deltaTime;

        std::vector<Entity*> staticEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        std::vector<Entity*> dynamicEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        std::vector<Entity*> renderEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        size_t renderablesCount;
        size_t staticsCount;
        size_t dynamicsCount;
        
        Scene* masterScene = m_SceneManager.GetMasterScene();
        Scene* activeScene;
        
        while (m_IsRunning)
        {
            if (!Input::PollInput())
                break;
            
            m_ScriptEngine.PreUpdate();
            
            renderablesCount = staticsCount = dynamicsCount = 0;

            m_Renderer->ClearFrame();
            
            deltaTime = 0.0f;// GetDeltaTime(frameStartTime, freqMs);

            masterScene->LogicUpdate(deltaTime);
            masterScene->GetPhysicsEntities(staticEntitiesBuffer, dynamicEntitiesBuffer, staticsCount, dynamicsCount);
            masterScene->GetRenderableEntities(renderEntitiesBuffer, renderablesCount);
            
            activeScene = m_SceneManager.GetActiveScene();
            
            if (auto sceneL = activeScene) {
                sceneL->LogicUpdate(deltaTime);
                sceneL->GetPhysicsEntities(staticEntitiesBuffer, dynamicEntitiesBuffer, staticsCount, dynamicsCount);
                sceneL->GetRenderableEntities(renderEntitiesBuffer, renderablesCount);
            }

            m_Physics.PhysicsUpdate(staticEntitiesBuffer, dynamicEntitiesBuffer, staticsCount, dynamicsCount, deltaTime);

            m_Renderer->DrawEntities(renderEntitiesBuffer, renderablesCount);
            
            m_Renderer->RenderFrame();
            
            m_SceneManager.OnEndOfFrame();

            float frameDuration = 0.0f;//(SDL_GetPerformanceCounter() - frameStartTime) * 1000.0f;

            //if (frameDuration < TARGET_FRAME_TIME)
            //{
            //    double delayMs = TARGET_FRAME_TIME - frameDuration;
            //    SDL_Delay((Uint32)delayMs);
            //}
            
            m_FrameCount++;
        }
        
        m_IsRunning = false;
        
        Destroy();
    }
    
    void Engine::Destroy() {
        Input::Destroy();
        m_ScriptEngine.Shutdown();

        delete m_Renderer;
        delete m_Window;
    }
    
    const Prefab* Engine::GetPrefab(GUID guid) {
        if (!m_HasInited) {
            Log("Trying to get prefab from non inited engine");
            return nullptr;
        }

        Asset prefabAsset = m_AssetManager.GetPrefabAsset(guid);
        return static_cast<const Prefab*>(prefabAsset.AssetPtr);
    }
}
