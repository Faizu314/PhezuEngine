#pragma once

#include <unordered_map>
#include <string>
#include <typeindex>
#include <functional>

#define REGISTER_COMPONENT(X) \
    bool Is_##X##_Registered = Phezu::RegisterComponent(#X, typeid(X), [](std::shared_ptr<Phezu::Entity> entity) { entity->AddComponent<X>(); });

namespace Phezu {
    
    class Entity;
    bool RegisterComponent(const std::string& typeName, std::type_index typeIndex, std::function<void(std::shared_ptr<Entity>)> constructor);
    
    class DataComponent {
    public:
        DataComponent() = delete;
        DataComponent(Entity* entity) : m_Entity(entity) {}
    public:
        Entity* GetEntity() const { return m_Entity; }
    protected:
        Entity* const m_Entity;
        
        
        /*------- Type Registries ------*/
    public:
        static const std::string& GetTypeName(std::type_index typeIndex);
        static std::function<void(std::shared_ptr<Entity>)> GetComponentConstructor(std::string componentName);
    private:
        static std::unordered_map<std::type_index, std::string>& GetTypeRegistry();
        static std::unordered_map<std::string, std::function<void(std::shared_ptr<Entity>)>>& GetConstructorRegistry();
        
        friend bool RegisterComponent(const std::string& typeName, std::type_index typeIndex, std::function<void(std::shared_ptr<Entity>)> constructor);
    };
}
