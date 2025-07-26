#include "Application.hpp"
#include "Phezu.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

Phezu::EngineConfig engineConfig {
    .Name = "PhezuRuntime",
    .ResolutionSettings = {
        .Width = 800,
        .Height = 600,
        .RenderScale = 1
    }
};

#ifdef __APPLE__

int main(int argc, const char* argv[]) {
    engineConfig.AllPaths = {
        .ExePath = PhezuRuntime::GetExePath(argv),
        .ProjectPath = PhezuRuntime::GetProjectPath(argv),
        .ScriptCoreDllPath = PhezuRuntime::GetScriptCoreDllPath(argv),
        .MonoCoreLibsPath = PhezuRuntime::GetMonoCoreLibsPath(argv)
    };
    
    if (engine.Init(engineConfig) != 0)
        return 1;
    
    engine.Run();
}

#elif _WIN32

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    engineConfig.AllPaths = {
        .ExePath = PhezuRuntime::GetExePath(),
        .ProjectPath = PhezuRuntime::GetProjectPath(),
        .ScriptCoreDllPath = PhezuRuntime::GetScriptCoreDllPath(),
        .MonoCoreLibsPath = PhezuRuntime::GetMonoCoreLibsPath()
    };
    
    if (engine.Init(engineConfig) != 0)
        return 1;
    
    engine.Run();

    return 0;
}

#endif
