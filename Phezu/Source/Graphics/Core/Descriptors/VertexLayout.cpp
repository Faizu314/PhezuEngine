#include "Core/Platform.hpp"

#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

	void VertexLayout::Push(VertexAttribute attrib) {
		m_Attributes.insert(std::pair(attrib.Semantic, attrib));
	}

	void VertexLayout::ComputeLayout() {
		EvaluateOffsetAndStride();
	}

	void VertexLayout::EvaluateOffsetAndStride() {
		m_Stride = 0;

		for (auto& kvp : m_Attributes) {
			kvp.second.Offset = m_Stride;

			unsigned int attribSize = GetVertexAttributeSize(kvp.second.AttributeType);
			unsigned int attribCount = GetVertexAttributeCount(kvp.second.AttributeCount);

			m_Stride += attribSize * attribCount;
		}
	}

	size_t VertexLayout::GetAttributesCount() const {
		return m_Attributes.size();
	}

	bool VertexLayout::HasSemantic(VertexSemantic semantic) const
	{
		return m_Attributes.find(semantic) != m_Attributes.end();
	}

	VertexAttribute VertexLayout::GetAttributeAt(VertexSemantic semantic) const {
		if (m_Attributes.find(semantic) == m_Attributes.end()) {
			Log("Assert semantic not found in vertex layout\n");
			return VertexAttribute();
		}

		return m_Attributes.at(semantic);
	}
}