#include "scene/components/DataComponent.hpp"

namespace Phezu {

    std::unordered_map<std::type_index, std::string> DataComponent::s_TypeRegistry;
    
    const std::string& DataComponent::GetTypeName(std::type_index typeIndex) {
        return s_TypeRegistry[typeIndex];
    }
    void DataComponent::RegisterType(std::type_index typeIndex, const std::string& typeName) {
        s_TypeRegistry[typeIndex] = typeName;
    }
}
