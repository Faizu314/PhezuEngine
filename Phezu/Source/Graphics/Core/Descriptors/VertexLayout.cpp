#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

	void VertexLayout::Push(VertexAttributeType type) {
		m_Attributes.push_back(type);
	}
}