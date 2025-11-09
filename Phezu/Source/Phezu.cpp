#include "Phezu.hpp"

namespace Phezu {
    
    Engine& CreateEngine() {
        if (Engine::s_Instance == nullptr)
            Engine::s_Instance = new Engine();
        
        return *Engine::s_Instance;
    }
    
    Engine* GetEngine() {
        return Engine::s_Instance;
    }
}
