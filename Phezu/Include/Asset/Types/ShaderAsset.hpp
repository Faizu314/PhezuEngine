#pragma once

#include "Asset/Core/Asset.hpp"

namespace Phezu {

	class ShaderAsset : public IAsset {
    public:
        ShaderAsset() = default;
    public:
        void Deserialize(const std::string& data) override;
	};

}