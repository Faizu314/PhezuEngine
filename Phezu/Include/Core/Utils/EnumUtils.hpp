#pragma once

#include <string>

#include "Scripting/Core/ScriptDefs.hpp"
#include "Assets/Core/Asset.hpp"
#include "Core/Types/VertexTypes.hpp"
#include "Core/Types/TextureTypes.hpp"

namespace Phezu {

	#define TO_STRING_UTIL(EnumType) EnumType To##EnumType(const std::string& enumStr); std::string ToString(EnumType enumValue)

	TO_STRING_UTIL(VertexSemantic);
	TO_STRING_UTIL(VertexAttributeType);
	TO_STRING_UTIL(VertexAttributeCount);
	TO_STRING_UTIL(TextureFilteringMode);
	TO_STRING_UTIL(TextureWrapMode);
	TO_STRING_UTIL(AssetSource);
	TO_STRING_UTIL(AssetType);
	TO_STRING_UTIL(ManagedType);

	unsigned int GetVertexAttributeSize(VertexAttributeType attribute);
	unsigned int GetVertexAttributeCount(VertexAttributeCount countEnum);
}