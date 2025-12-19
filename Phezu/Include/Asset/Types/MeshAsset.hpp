#pragma once

#include <string>
#include <vector>

#include "Core/Types/Buffer.hpp"
#include "Asset/Core/Asset.hpp"

namespace Phezu {

    struct SerializedBuffer {
        VertexSemantic Semantic;
        VertexAttributeType Type;
        VertexAttributeCount Count;
        bool Normalized;
        std::vector<float> Data;
    };

	class MeshAsset : public IAsset {
    public:
        MeshAsset() = default;
    public:
        void Deserialize(const std::string& data) override;
    private:
        GUID m_Guid;
        std::vector<SerializedBuffer> m_VertexBuffers;
        std::vector<unsigned int> m_IndexBuffer;
        size_t m_Stride = 0;
        size_t m_VertexCount = 0;
	};
}
