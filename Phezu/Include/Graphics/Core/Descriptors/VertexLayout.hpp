#pragma once

#include "Graphics/Core/GraphicsTypes.hpp"
#include <vector>

namespace Phezu {

	class VertexLayout {
	public:
		VertexLayout() = default;
	public:
		void Push(VertexAttributeType type);
	private:
		std::vector<VertexAttributeType> m_Attributes;
	};
}