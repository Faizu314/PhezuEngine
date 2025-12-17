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
		size_t Offset = 0;
		bool Normalized = true;
	};

	enum class BufferType {
		Static,
		Dynamic
	};
}