#pragma once

#include <string>
#include <vector>

#include "Core/Types/VertexTypes.hpp"
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
    public:
        GUID Guid;
        std::vector<SerializedBuffer> VertexBuffers;
        std::vector<unsigned int> IndexBuffer;
        size_t Stride = 0;
        size_t VertexCount = 0;
	};
}
