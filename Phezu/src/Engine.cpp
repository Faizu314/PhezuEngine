#include <string>

#include "Engine.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "scene/Scene.hpp"
#include "scene/Prefab.hpp"
#include "Logger.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

namespace Phezu {
    
    static const size_t ENTITIES_BUFFER_SIZE = 128;

    Engine* Engine::s_Instance = nullptr;
    
    Engine::Engine() : m_HasInited(false), m_IsRunning(false),
        m_FrameCount(0), m_AssetManager(this), m_SceneManager(this),
        m_Physics(this), m_Renderer(nullptr),
        m_Window(nullptr), m_ScriptEngine(this) {}
    
    int Engine::Init(EngineConfig& config) {
        if (m_HasInited) {
            Log("Trying to init engine twice");
            return 1;
        }

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            Log("Couldn't initialize SDL: %s\n", SDL_GetError());
            return 2;
        }

        if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0){
            Log("Couldn't initialize SDL Image: %s\n", SDL_GetError());
            return 3;
        }

        if (TTF_Init() < 0) {
            Log("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
            return 4;
        }

        m_HasInited = true;
        m_ExePath = config.AllPaths.ExePath;
        m_ProjectPath = config.AllPaths.ProjectPath;
        m_ScriptCoreDllPath = config.AllPaths.ScriptCoreDllPath;
        m_MonoCoreLibsPath = config.AllPaths.MonoCoreLibsPath;
        m_Window = new Window(config.Name, config.ResolutionSettings.Width, config.ResolutionSettings.Height, config.ResolutionSettings.RenderScale);
        m_Renderer = new Renderer(this, *m_Window);
        m_AssetManager.Init(m_ProjectPath);
        m_SceneManager.Init();
        m_ScriptEngine.Init();
        Input::Init();
        
        return 0;
    }
    
    float GetDeltaTime(Uint64& prevTime, Uint64& freqMs) {
        Uint64 currTime = SDL_GetPerformanceCounter();
        float deltaTime = (currTime - prevTime) / (float)freqMs;
        prevTime = SDL_GetPerformanceCounter();
        
        return deltaTime;
    }
    
    void Engine::Run() {
        if (!m_HasInited || m_Window == nullptr || m_Renderer == nullptr) {
            Log("Trying to run non inited engine");
            return;
        }
        
        m_IsRunning = true;
        
        m_Renderer->SetActiveCamera(m_SceneManager.GetActiveCamera());
        m_SceneManager.OnStartGame();
        
        Uint64 prevTime = SDL_GetPerformanceCounter();
        Uint64 freqMs = SDL_GetPerformanceFrequency();
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
            
            deltaTime = GetDeltaTime(prevTime, freqMs);
            
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
        
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
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
