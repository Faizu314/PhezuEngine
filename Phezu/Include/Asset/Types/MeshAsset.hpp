#pragma once

#include <string>

#include "Core/Types/Buffer.hpp"
#include "Asset/Core/Asset.hpp"

namespace Phezu {

	class MeshAsset : public IAsset {
    public:
        MeshAsset() = default;
    public:
        void Deserialize(const std::string& data) override;
    private:
        GUID m_Guid;
        Buffer m_VertexBuffer;
        Buffer m_IndexBuffer;
	};
}
