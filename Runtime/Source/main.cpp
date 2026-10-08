#include "Phezu.hpp"
#include "Application.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

Phezu::EngineArgs engineArgs {
    {},
    {
        "PhezuRuntime",
        800,
        600,
        1
    }
};

#if defined(PZ_PLATFORM_WEB)

extern "C"
{
    void Phezu_Init() {
        engineArgs.AllPaths = {
            Phezu::Runtime::GetAssetsPath(),
            "",
            ""
        };

        engine.Init(engineArgs);
    }

    void Phezu_StartSim() {
        engine.StartSim();
    }

    void Phezu_UpdateSim(float deltaTime) {
        engine.UpdateSim(deltaTime);
    }

    void Phezu_StopSim() {
        engine.StopSim();
    }
}

#elif defined(PZ_PLATFORM_WINDOWS)

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    engineArgs.AllPaths = {
        Phezu::Runtime::GetAssetsPath(),
        Phezu::Runtime::GetScriptCoreDllPath(),
        Phezu::Runtime::GetMonoCoreLibsPath()
    };

    engine.Init(engineArgs);
    engine.Run();

    return 0;
}

#endif
