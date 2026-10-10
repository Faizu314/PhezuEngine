#include "Core/Defs/Assert.hpp"
#include "Core/Platform.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"
#include "Core/Utils/EnumUtils.hpp"

namespace Phezu {

	void VertexLayout::Push(VertexAttribute attrib) {
		m_Attributes.push_back(attrib);
	}

	void VertexLayout::ComputeLayout() {
		EvaluateOffsetAndStride();
	}

	void VertexLayout::EvaluateOffsetAndStride() {
		m_Stride = 0;

		for (auto& attrib : m_Attributes) {
			attrib.Offset = m_Stride;

			unsigned int attribSize = GetVertexAttributeSize(attrib.AttributeType);
			unsigned int attribCount = GetVertexAttributeCount(attrib.AttributeCount);

			m_Stride += attribSize * attribCount;
		}
	}

	size_t VertexLayout::GetAttributesCount() const {
		return m_Attributes.size();
	}

	bool VertexLayout::HasSemantic(VertexSemantic semantic) const
	{
		for (auto& attrib : m_Attributes) {
			if (attrib.Semantic == semantic)
				return true;
		}

		return false;
	}

	VertexAttribute VertexLayout::GetAttributeAt(VertexSemantic semantic) const {
		for (auto& attrib : m_Attributes) {
			if (attrib.Semantic == semantic)
				return attrib;
		}

		PZ_ASSERT(false, "Semantic not found in vertex layout.\n");
	}
}