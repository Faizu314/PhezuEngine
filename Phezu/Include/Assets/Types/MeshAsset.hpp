// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <string>
#include <vector>

#include "Core/Types/Types.hpp"
#include "Assets/Core/Asset.hpp"
#include "Graphics/Core/Graphics.hpp"

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
        AssetType GetAssetType() override { return AssetType::Mesh; }
        void Deserialize(const std::string& data) override;
    public:
        std::vector<SerializedBuffer> VertexBuffers;
        std::vector<unsigned int> IndexBuffer;
        size_t Stride = 0;
        size_t VertexCount = 0;
	};
}
