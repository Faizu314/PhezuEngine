#pragma once

#include "Core/Types/Color.hpp"
#include "Asset/Types/MaterialAsset.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        RenderData(Entity* entity) : DataComponent(entity) {}
    public:
        void SetMaterialHandle(AssetHandle materialHandle) { m_MaterialHandle = materialHandle; }
        AssetHandle GetMaterialHandle() { return m_MaterialHandle; }
    private:
        AssetHandle m_MaterialHandle;
    };
}
