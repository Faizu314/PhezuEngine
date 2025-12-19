#pragma once

#include "nlohmann/json.hpp"

#include "Core/Types/VertexTypes.hpp"
#include "Asset/Types/MeshAsset.hpp"

namespace Phezu {

    void MeshAsset::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        m_Guid = j["Guid"].get<uint64_t>();
        m_VertexCount = j["VertexCount"].get<size_t>();

        size_t totalBuffersSize = 0;
        m_Stride = 0;
        size_t index = 0;

        for (const auto& bufferJson : j["VertexData"]) {
            m_VertexBuffers.push_back(SerializedBuffer());
            SerializedBuffer& buffer = m_VertexBuffers[index++];
            buffer.Semantic = ToVertexSemantic(bufferJson["Semantic"].get<std::string>());
            buffer.Type = ToVertexAttributeType(bufferJson["Type"].get<std::string>());
            buffer.Count = ToVertexAttributeCount(bufferJson["Count"].get<std::string>());
            buffer.Normalized = bufferJson.value("Normalize", false);
            buffer.Data = bufferJson["Data"].get<std::vector<float>>();

            size_t elementSize = GetVertexAttributeSize(buffer.Type);
            size_t attribCount = GetVertexAttributeCount(buffer.Count);

            if ((data.size() / attribCount) != m_VertexCount)
                Log("Should assert here\n");

            m_Stride += elementSize * attribCount;
        }

        m_IndexBuffer = j["IndexData"].get<std::vector<unsigned int>>();
    }
}