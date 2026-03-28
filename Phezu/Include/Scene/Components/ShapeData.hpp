// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Graphics/Data/Mesh.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class ShapeData : public DataComponent {
    public:
        ShapeData(Entity* entity) : DataComponent(entity) { }
    public:
        void SetMeshHandle(ResourceHandle meshHandle) { m_MeshHandle = meshHandle; }
        ResourceHandle GetMeshHandle() { return m_MeshHandle; }
    private:
        ResourceHandle m_MeshHandle;
    };
}
