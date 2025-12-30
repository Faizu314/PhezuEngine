#pragma once

#include <unordered_map>
#include <variant>

#include "Asset/Core/Asset.hpp"
#include "Core/Types/Types.hpp"
#include "Core/Types/Color.hpp"
#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Vector3.hpp"

namespace Phezu {

	class MaterialAsset : public IAsset {
	public:
		MaterialAsset(GUID guid) : IAsset(guid) {}
	public:
		void Deserialize(const std::string& data) override;
	public:
		GUID ShaderRef;
		std::unordered_map<std::string, GUID> Textures;
		std::unordered_map<std::string, MaterialProperty> Parameters;
	};

}