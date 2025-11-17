#include "Phezu.hpp"
#include "Application.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

Phezu::EngineArgs engineArgs {
    "PhezuRuntime",
    {},
    { 800, 600, 1 }
};

#ifdef __APPLE__

int main(int argc, const char* argv[]) {
    engineArgs.AllPaths = {
        .ExePath = Phezu::Runtime::GetExePath(argv),
        .ProjectPath = Phezu::Runtime::GetProjectPath(argv),
        .ScriptCoreDllPath = Phezu::Runtime::GetScriptCoreDllPath(argv),
        .MonoCoreLibsPath = Phezu::Runtime::GetMonoCoreLibsPath(argv)
    };
    
    if (engine.Init(engineArgs) != 0)
        return 1;
    
    engine.Run();
}

#elif _WIN32

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    engineArgs.AllPaths = {
        Phezu::Runtime::GetExePath(),
        Phezu::Runtime::GetAssetsPath(),
        Phezu::Runtime::GetScriptCoreDllPath(),
        Phezu::Runtime::GetMonoCoreLibsPath()
    };
    engineArgs.WinArgs = {
        hInstance,
        nCmdShow
    };

    int err = engine.Init(engineArgs);
    
    if (err != 0)
        return err;
    
    engine.Run();

    return 0;
}

#endif
