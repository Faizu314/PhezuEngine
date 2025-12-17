#pragma once

#include <stdint.h>

#include "Core/Types/VertexTypes.hpp"

namespace Phezu {

	struct VertexAttribute {
		VertexAttribute() = default;

		VertexAttribute(VertexSemantic semantic, VertexAttributeType type, VertexAttributeCount count, bool normalized = true)
			: Semantic(semantic), AttributeType(type), AttributeCount(count), Normalized(normalized) {}

		VertexSemantic Semantic;
		VertexAttributeType AttributeType;
		VertexAttributeCount AttributeCount;
		unsigned int Index = 0;
		unsigned int Offset = 0;
		bool Normalized;
	};

	enum class BufferType {
		Static,
		Dynamic
	};
}