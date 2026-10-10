#include <string>

#include "Core/Defs/Assert.hpp"
#include "Core/Engine.hpp"

namespace Phezu {

    static const double TARGET_FRAME_TIME = 1.0f / 60.0f;
    static const size_t ENTITIES_BUFFER_SIZE = 128;

    Engine* Engine::s_Instance = nullptr;
    
    Engine::Engine() : m_HasInited(false), m_IsRunning(false),
        m_FrameCount(0), m_Platform(nullptr) { }
    
    void Engine::Init(EngineArgs& args) {
        PZ_ASSERT(!m_HasInited, "Trying to init engine twice");

        m_AssetsPath = args.AllPaths.AssetsPath;
        m_ScriptCoreDllPath = args.AllPaths.ScriptCoreDllPath;
        m_MonoCoreLibsPath = args.AllPaths.MonoCoreLibsPath;

        m_Platform = CreatePlatform();
        m_Platform->Init(args.WindowArgs);

        m_AssetManager.Init(m_AssetsPath);
        m_ResourceManager.Init(&m_AssetManager, m_Platform->GetGraphicsApi());
        m_Renderer.Init({ m_Platform->GetWindow(), m_Platform->GetGraphicsApi(), &m_ResourceManager }, RenderTarget::Default());
        m_ScriptEngine.Init(this);
        m_SceneManager.Init({ &m_AssetManager, &m_ResourceManager, &m_ScriptEngine });

        m_HasInited = true;
    }
    
    void Engine::Run() {
        PZ_ASSERT(m_HasInited, "Trying to run non inited engine");
        
        double deltaTime;
        double frameStartTime;
        double prevFrameTime = m_Platform->GetTime();

        std::vector<Entity*> renderEntitiesBuffer(ENTITIES_BUFFER_SIZE);
        size_t renderablesCount;

        m_SceneManager.OnStartGame();
        m_IsRunning = true;
        
        while (m_IsRunning)
        {
            frameStartTime = m_Platform->GetTime();
            deltaTime = frameStartTime - prevFrameTime;
            float deltaTimeF = static_cast<float>(deltaTime);

            m_Platform->PollEvents();
            m_ScriptEngine.OnUpdate(deltaTimeF);
            m_SceneManager.Update(deltaTimeF);
            //physics update
            m_SceneManager.GetRenderableEntities(renderEntitiesBuffer, renderablesCount);
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

    void Engine::StartSim() {
        PZ_ASSERT(!m_IsRunning, "Trying to start simulation twice");

        m_RenderEntitiesBuffer.reserve(ENTITIES_BUFFER_SIZE);
        m_SceneManager.OnStartGame();
        m_IsRunning = true;
        m_FrameCount = 0;
    }

    void Engine::StopSim() {
        PZ_ASSERT(m_IsRunning, "Trying to stop simulation without starting");

        m_IsRunning = false;

        Destroy();
    }

    void Engine::UpdateSim(float deltaTime) {
        PZ_ASSERT(m_IsRunning, "Trying to update simulation without starting");

        size_t renderablesCount;

        m_Platform->PollEvents();
        m_ScriptEngine.OnUpdate(deltaTime);
        m_SceneManager.Update(deltaTime);
        //physics update
        m_SceneManager.GetRenderableEntities(m_RenderEntitiesBuffer, renderablesCount);
        m_Renderer.DrawScene(m_RenderEntitiesBuffer, renderablesCount, m_SceneManager.GetActiveCamera());
        m_Platform->Update();

        m_SceneManager.OnEndOfFrame();

        m_FrameCount++;
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
