#pragma once

#include "Core/Types/Color.hpp"
#include "Asset/Types/ShaderAsset.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        RenderData(Entity* entity) : DataComponent(entity) {}
    public:
        void SetShaderHandle(AssetHandle<ShaderAsset> handle) { m_ShaderHandle = handle; }
        AssetHandle<ShaderAsset> GetShaderHandle() { return m_ShaderHandle; }
        void SetTint(Color tint) { m_Tint = tint; }
        Color GetTint() { return m_Tint; }
    private:
        //Texture
        void* m_Sprite;
        AssetHandle<ShaderAsset> m_ShaderHandle;
        Color m_Tint;
    };
}
