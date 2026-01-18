#include <string>

#include "Core/Engine.hpp"

namespace Phezu {
    
    static const double MAX_DELTA_TIME = 1.0f / 20.0f;
    static const double TARGET_FRAME_TIME = 1.0f / 60.0f;
    static const size_t ENTITIES_BUFFER_SIZE = 128;

    Engine* Engine::s_Instance = nullptr;
    
    Engine::Engine() : m_HasInited(false), m_IsRunning(false),
        m_FrameCount(0), m_AssetManager(this), m_SceneManager(this),
        m_Physics(this), m_Renderer(), m_Platform(nullptr),
        m_ScriptEngine(this) {}
    
    int Engine::Init(EngineArgs& args) {
        if (m_HasInited) {
            Log("Trying to init engine twice");
            return 1;
        }

        m_HasInited = true;
        m_ExePath = args.AllPaths.ExePath;
        m_AssetsPath = args.AllPaths.AssetsPath;
        m_ScriptCoreDllPath = args.AllPaths.ScriptCoreDllPath;
        m_MonoCoreLibsPath = args.AllPaths.MonoCoreLibsPath;

        m_Platform = CreatePlatform();
        m_Platform->Init(args.WindowArgs);

        m_AssetManager.Init(m_AssetsPath);
        m_ResourceManager.Init(&m_AssetManager, m_Platform->GetGraphicsApi());
        m_Renderer.Init({ m_Platform->GetWindow(), m_Platform->GetGraphicsApi(), &m_ResourceManager }, RenderTarget::Default());
        m_SceneManager.Init();
        m_ScriptEngine.Init();
        
        return 0;
    }
    
    void Engine::Run() {
        if (!m_HasInited) {
            Log("Trying to run non inited engine");
            return;
        }
        
        double deltaTime;
        double frameStartTime;
        double prevFrameTime = m_Platform->GetTime();

        std::vector<Entity*> staticEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        std::vector<Entity*> dynamicEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        std::vector<Entity*> renderEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        size_t renderablesCount;
        size_t staticsCount;
        size_t dynamicsCount;

        m_SceneManager.OnStartGame();
        m_IsRunning = true;
        
        while (m_IsRunning)
        {
            frameStartTime = m_Platform->GetTime();
            deltaTime = frameStartTime - prevFrameTime;
            renderablesCount = staticsCount = dynamicsCount = 0;
            float deltaTimeF = static_cast<float>(deltaTime);

            m_Platform->PollEvents();
            m_ScriptEngine.OnUpdate(deltaTimeF);
            m_SceneManager.Update(deltaTimeF);
            m_SceneManager.GetPhysicsEntities(staticEntitiesBuffer, dynamicEntitiesBuffer, staticsCount, dynamicsCount);
            m_SceneManager.GetRenderableEntities(renderEntitiesBuffer, renderablesCount);
            m_Physics.PhysicsUpdate(staticEntitiesBuffer, dynamicEntitiesBuffer, staticsCount, dynamicsCount, deltaTimeF);
            m_Renderer.DrawScene(renderEntitiesBuffer, renderablesCount, m_SceneManager.GetActiveCamera());
            m_Platform->Update();
            
            m_SceneManager.OnEndOfFrame();

            double frameDuration = m_Platform->GetTime() - frameStartTime;

            if (frameDuration < TARGET_FRAME_TIME)
            {
                double delay = TARGET_FRAME_TIME - frameDuration;
                m_Platform->Wait(static_cast<float>(delay));
            }
            
            m_FrameCount++;
            prevFrameTime = frameStartTime;
        }
        
        m_IsRunning = false;
        
        Destroy();
    }
    
    void Engine::Destroy() {
        m_ScriptEngine.Shutdown();
        //m_SceneManager.Shutdown();
        m_Renderer.Destroy();
        m_ResourceManager.Destroy();
        m_Platform->Destroy();

        delete m_Platform;
    }
}
