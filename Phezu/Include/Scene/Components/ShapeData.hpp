// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Graphics/Data/Mesh.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class ShapeData : public DataComponent {
    public:
        ShapeData(Entity* entity) : DataComponent(entity), m_Mesh(nullptr) { }
    public:
        void SetMesh(const Mesh* mesh) { m_Mesh = mesh; }
        const Mesh* GetMesh() { return m_Mesh; }
    private:
        const Mesh* m_Mesh;
    };
}
