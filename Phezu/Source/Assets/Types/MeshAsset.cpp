#pragma once

#include "nlohmann/json.hpp"

#include "Core/Platform.hpp"
#include "Core/Types/Types.hpp"
#include "Assets/Types/MeshAsset.hpp"

namespace Phezu {

    void MeshAsset::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        VertexCount = j["VertexCount"].get<size_t>();

        size_t totalBuffersSize = 0;
        Stride = 0;
        size_t index = 0;

        for (const auto& bufferJson : j["VertexData"]) {
            VertexBuffers.push_back(SerializedBuffer());
            SerializedBuffer& buffer = VertexBuffers[index++];
            buffer.Semantic = ToVertexSemantic(bufferJson["Semantic"].get<std::string>());
            buffer.Type = ToVertexAttributeType(bufferJson["Type"].get<std::string>());
            buffer.Count = ToVertexAttributeCount(bufferJson["Count"].get<std::string>());
            buffer.Normalized = bufferJson.value("Normalize", false);
            buffer.Data = bufferJson["Data"].get<std::vector<float>>();

            size_t elementSize = GetVertexAttributeSize(buffer.Type);
            size_t attribCount = GetVertexAttributeCount(buffer.Count);

            if ((buffer.Data.size() / attribCount) != VertexCount)
                Log("Should assert here\n");

            Stride += elementSize * attribCount;
        }

        IndexBuffer = j["IndexData"].get<std::vector<unsigned int>>();
    }
}