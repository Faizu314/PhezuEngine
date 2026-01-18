#pragma once

#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class Material;

    class RenderData : public DataComponent {
    public:
        RenderData(Entity* entity) : DataComponent(entity), m_Material(nullptr) {}
    public:
        void SetMaterial(Material* material) { m_Material = material; }
        Material* GetMaterial() { return m_Material; }
    private:
        Material* m_Material;
    };
}
