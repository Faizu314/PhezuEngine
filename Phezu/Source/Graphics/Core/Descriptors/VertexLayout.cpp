#include "Core/Platform.hpp"

#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

	VertexLayout::VertexLayout(std::initializer_list<VertexAttribute> list) {
		for (auto& attrib : list) {
			m_Attributes.insert(std::pair(attrib.Semantic, attrib));
		}

		EvaluateOffsetAndStride();
	}

	void VertexLayout::EvaluateOffsetAndStride() {
		m_Stride = 0;
		unsigned int offset = 0;

		for (auto& kvp : m_Attributes) {
			kvp.second.Offset = offset;

			unsigned int attribSize = GetVertexAttributeSize(kvp.second.AttributeType);
			unsigned int attribCount = GetVertexAttributeCount(kvp.second.AttributeCount);

			m_Stride += attribSize;
			offset += attribSize * attribCount;
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