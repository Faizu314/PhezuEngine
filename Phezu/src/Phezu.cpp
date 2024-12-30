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
    
    std::weak_ptr<Entity> CreateEntity() {
        Engine* instance = GetEngine();
        
        if (instance == nullptr) {
            //TODO: Logging
            return std::weak_ptr<Entity>();
        }
        
        return instance->CreateEntity();
    }
    
    std::weak_ptr<Entity> CreateEntity(uint64_t prefabID) {
        Engine* instance = GetEngine();
        
        if (instance == nullptr) {
            //TODO: Logging
            return std::weak_ptr<Entity>();
        }
        
        return instance->CreateEntity(prefabID);
    }
    
    void LoadScene(const std::string& sceneName) {
        Engine* instance = GetEngine();
        
        if (instance == nullptr) {
            //TODO: Logging
            return;
        }
        
        instance->LoadScene(sceneName);
    }
    
    void Destroy(Entity* entity) {
        if (entity == nullptr)
            return;
        if (auto scene = entity->m_Scene.lock())
            scene->DestroyEntity(entity->GetEntityID());
    }
    
    const InputData& GetInput() {
        Engine* instance = GetEngine();
        
        if (instance == nullptr) {
            //TODO: Logging
            static const InputData defaultInput{};
            return defaultInput;
        }
        
        return instance->GetInput();
    }
    
    long long unsigned int GetFrameCount() {
        Engine* instance = GetEngine();
        
        if (instance == nullptr) {
            //TODO: Logging
            return 0;
        }
        
        return instance->GetFrameCount();
    }
    
    void UnsubscribeToOnSceneLoaded(void* subscriber) {
        Engine* engine = GetEngine();
        
        if (engine == nullptr) {
            //TODO: Logging
            return;
        }
        
        engine->GetSceneManager().UnsubscribeToOnSceneLoaded(subscriber);
    }
    
    bool RegisterComponent(const std::string& typeName, std::type_index typeIndex, std::function<void(std::shared_ptr<Entity>)> constructor) {
        auto& typeRegistry = DataComponent::GetTypeRegistry();
        auto& constructorRegistry = DataComponent::GetConstructorRegistry();
        
        typeRegistry[typeIndex] = typeName;
        constructorRegistry[typeName] = constructor;
        
        return true;
    }
}
