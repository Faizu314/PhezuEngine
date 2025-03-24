#include "Application.hpp"
#include "Phezu.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

#ifdef __APPLE__

int main(int argc, const char* argv[]) {
    engine.Init(PhezuRuntime::GetProjectPath(argv), "PhezuRuntime", 800, 600);
    
    engine.Run();
}

#elif _WIN32

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    engine.Init(PhezuRuntime::GetProjectPath(), "PhezuRuntime", 800, 600);
    
    engine.Run();

    return 0;
}

#endif
