#pragma once

#include <unordered_map>
#include <string>
#include <typeindex>

#define REGISTER_COMPONENT(X) \
    bool Is_##X##_Registered = Phezu::RegisterComponent<X>(#X, [](std::shared_ptr<Phezu::Entity> entity) { entity->AddComponent<X>(); });

namespace Phezu {
    
    class Entity;
    
    class DataComponent {
    public:
        DataComponent() = delete;
        DataComponent(Entity* entity) : m_Entity(entity) {}
    public:
        static const std::string& GetTypeName(std::type_index typeIndex);
        static void RegisterType(std::type_index typeIndex, const std::string& typeName);
    public:
        Entity* GetEntity() const { return m_Entity; }
    protected:
        Entity* const m_Entity;
    private:
        static std::unordered_map<std::type_index, std::string> s_TypeRegistry;
    };
}
