#pragma once

#include "Asset/Types/MeshAsset.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class ShapeData : public DataComponent {
    public:
        ShapeData(Entity* entity);
    public:
        void SetMeshHandle(AssetHandle meshHandle) { m_MeshHandle = meshHandle; }
        AssetHandle GetMeshHandle() { return m_MeshHandle; }
    private:
        AssetHandle m_MeshHandle;
    };
}
