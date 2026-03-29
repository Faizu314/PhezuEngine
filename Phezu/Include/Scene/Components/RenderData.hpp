// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Scene/Components/DataComponent.hpp"
#include "Graphics/Core/Graphics.hpp"

namespace Phezu {
    
    class RendererData : public DataComponent {
    public:
        RendererData(Entity* entity) : DataComponent(entity) {}
    public:
        void SetMaterialHandle(ResourceHandle materialHandle) { m_MaterialHandle = materialHandle; }
        ResourceHandle GetMaterialHandle() { return m_MaterialHandle; }
    private:
        ResourceHandle m_MaterialHandle;
    };
}
