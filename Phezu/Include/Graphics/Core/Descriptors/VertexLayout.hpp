#pragma once

#include "Graphics/Core/GraphicsTypes.hpp"
#include <vector>

namespace Phezu {

	class VertexLayout {
	public:
		VertexLayout() = default;
	public:
		void Push(VertexAttributeType type, VertexAttributeCount count, bool normalized = true);
		size_t GetAttributesCount() const;
		VertexAttribute GetAttributeAt(int index) const;
	private:
		std::vector<VertexAttribute> m_Attributes;
	};
}