#include "Core/Platform.hpp"

#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

	void VertexLayout::Push(VertexAttributeType type, VertexAttributeCount count, bool normalized) {
		m_Attributes.emplace_back(type, count, normalized);
	}

	size_t VertexLayout::GetAttributesCount() const {
		return m_Attributes.size();
	}

	VertexAttribute VertexLayout::GetAttributeAt(int index) const {
		if (index < 0 || index >= m_Attributes.size()) {
			Log("Assert invalid index of vertex attribute\n");
		}

		return m_Attributes[index];
	}
}