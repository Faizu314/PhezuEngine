#include "Application.hpp"
#include "Phezu.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

#ifdef __APPLE__

int main(int argc, const char* argv[]) {
    engine.Init(PhezuRuntime::GetProjectPath(argv), "PhezuRuntime", 800, 600);
    
    engine.Run();
}

#elif _WIN32

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    engine.Init(PhezuRuntime::GetProjectPath(argv), "PhezuRuntime", 800, 600);
    
    engine.Run();

    return 0;
}

#endif
