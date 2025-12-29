#pragma once

#include "Core/Types/TextureTypes.hpp"
#include "Asset/Core/Asset.hpp"

namespace Phezu {

	class TextureAsset : public IAsset {
	public:
		TextureAsset(GUID guid) : IAsset(guid) {}
	public:
		void Deserialize(const std::string& data) override;
	public:
		GUID ImageRef;
		TextureFilteringMode FilteringMode = TextureFilteringMode::Point;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;
	};

}