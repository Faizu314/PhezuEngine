#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    const std::string& DataComponent::GetTypeName(std::type_index typeIndex) {
        return GetTypeRegistry()[typeIndex];
    }
    
    std::function<void(std::shared_ptr<Entity>)> DataComponent::GetComponentConstructor(std::string componentName) {
        auto& registry = GetConstructorRegistry();
        if (registry.find(componentName) == registry.end()) {
            //TODO: Logging
            return nullptr;
        }
        
        return registry[componentName];
    }
    
    std::unordered_map<std::type_index, std::string>& DataComponent::GetTypeRegistry() {
        static std::unordered_map<std::type_index, std::string> s_TypeRegistry;
        return s_TypeRegistry;
    }
    
    std::unordered_map<std::string, std::function<void(std::shared_ptr<Entity>)>>& DataComponent::GetConstructorRegistry() {
        static std::unordered_map<std::string, std::function<void(std::shared_ptr<Entity>)>> s_ConstructorRegistry;
        return s_ConstructorRegistry;
    }
}
