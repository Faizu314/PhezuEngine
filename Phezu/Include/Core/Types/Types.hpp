#pragma once

#include <stdint.h>
#include <string>
#include <variant>

#include "Core/Types/Color.hpp"
#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Vector3.hpp"

namespace Phezu {

	enum class TextureWrapMode {
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFilteringMode {
		Point,
		Bilinear
	};

	struct SamplerDesc {
		TextureWrapMode WrapMode;
		TextureFilteringMode FilteringMode;
	};

	enum class VertexSemantic : uint8_t {
		Position,
		Normal,
		Color,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		Custom0,
		Custom1,
		Custom2,
		Custom3
	};

	enum class VertexAttributeType : uint8_t {
		Float,
		Int,
		UInt,
		Byte,
		UByte
	};

	enum class VertexAttributeCount : uint8_t {
		One = 1,
		Two = 2,
		Three = 3,
		Four = 4
	};

	VertexSemantic ToVertexSemantic(std::string semanticStr);
	VertexAttributeType ToVertexAttributeType(std::string attributeTypeStr);
	VertexAttributeCount ToVertexAttributeCount(std::string attributeCountStr);

	unsigned int GetVertexAttributeSize(VertexAttributeType attribute);
	unsigned int GetVertexAttributeCount(VertexAttributeCount countEnum);

	enum class MaterialParameterType : uint8_t {
		Float,
		Float2,
		Float3,
		Color,
		Int,
		Bool
	};

	using MaterialParameterValue = std::variant<float, Vector2, Vector3, Color, int, bool>;

	struct MaterialParameter {
		MaterialParameterType Type;
		MaterialParameterValue Value;
	};
}