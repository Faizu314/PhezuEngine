#pragma once

#include <unordered_map>

#include "Core/Types/Types.hpp"
#include "Asset/Core/Asset.hpp"

namespace Phezu {

	class ShaderAsset : public IAsset {
    public:
        ShaderAsset(GUID guid) : IAsset(guid) {}
    public:
        void Deserialize(const std::string& data) override;
    private:
        std::string FindVertexInputBlock(const std::string& src, size_t* start, size_t* end);
        std::string ParseVertexInputBlock(const std::string& block);
        std::string ReplaceVertexInputBlock(const std::string& src, size_t blockStart, size_t blockEnd, const std::string& replacement);
    public:
        std::string Language;
        std::unordered_map<VertexSemantic, unsigned int> Semantics;
        std::string VertexSource;
        std::string FragmentSource;
	};

}