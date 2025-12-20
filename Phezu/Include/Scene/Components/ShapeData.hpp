#pragma once

#include "Asset/Types/MeshAsset.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class ShapeData : public DataComponent {
    public:
        ShapeData(Entity* entity);
    public:
        void SetMeshHandle(AssetHandle<MeshAsset> handle) { m_MeshHandle = handle; }
        AssetHandle<MeshAsset> GetMeshHandle() { return m_MeshHandle; }
    private:
        AssetHandle<MeshAsset> m_MeshHandle;
    };
}
