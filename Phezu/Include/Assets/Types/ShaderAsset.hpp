#pragma once

#include <unordered_map>
#include <string>

#include "Assets/Core/Asset.hpp"
#include "Core/Types/Types.hpp"

namespace Phezu {

	class ShaderAsset : public IAsset {
    public:
        AssetType GetAssetType() override { return AssetType::Shader; }
        void Deserialize(const std::string& data) override;
    private:
        std::string FindVertexInputBlock(const std::string& src, size_t* start, size_t* end);
        std::string ParseVertexInputBlock(const std::string& block);
        std::string ReplaceVertexInputBlock(const std::string& src, size_t blockStart, size_t blockEnd, const std::string& replacement);
    public:
        std::unordered_map<VertexSemantic, unsigned int> Semantics;
        std::string VertexSource;
        std::string FragmentSource;
	};

}