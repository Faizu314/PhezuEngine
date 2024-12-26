#include "Phezu.hpp"
#include "Game.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

#ifdef __APPLE__

int main(int argc, const char* argv[]) {
    engine.Init("Tetris", 800, 600);
    
    //PrepareScenes(engine);
    
    engine.Run();
}

#elif _WIN32

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    engine.Init("Phezu", 800, 600);

    //PrepareScenes(engine);

    engine.Run();

    return 0;
}

#endif
