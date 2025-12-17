#include "Core/Platform.hpp"

#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

	unsigned int GetVertexAttributeSize(const VertexAttribute& attribute) {
		int count = GetVertexAttributeCount(attribute.AttributeCount);

		switch (attribute.AttributeType) {
			case VertexAttributeType::Float:
				return sizeof(float) * count;

			case VertexAttributeType::Int:
				return sizeof(int) * count;
			case VertexAttributeType::UInt:
				return sizeof(unsigned int) * count;

			case VertexAttributeType::Byte:
			case VertexAttributeType::UByte:
				return count;

			default: {
				Log("Should assert here\n");
				return 0;
			}
		}
	}

	unsigned int GetVertexAttributeCount(VertexAttributeCount countEnum) {
		switch (countEnum) {
			case VertexAttributeCount::One:
				return 1;
			case VertexAttributeCount::Two:
				return 2;
			case VertexAttributeCount::Three:
				return 3;
			case VertexAttributeCount::Four:
				return 4;

			default: {
				Log("Should assert here\n");
				return 0;
			}
		}
	}


	VertexLayout::VertexLayout(std::initializer_list<VertexAttribute> list) {
		int index = 0;

		for (auto& attrib : list) {
			VertexAttribute copy = attrib;
			copy.Index = index;
			m_Attributes.insert(std::pair(attrib.Semantic, copy));

			index++;
		}

		EvaluateOffsetAndStride();
	}

	void VertexLayout::EvaluateOffsetAndStride() {
		m_Stride = 0;
		unsigned int offset = 0;

		for (auto& kvp : m_Attributes) {
			kvp.second.Offset = offset;

			unsigned int attribSize = GetVertexAttributeSize(kvp.second);

			m_Stride += attribSize;
			offset += attribSize;
		}
	}

	size_t VertexLayout::GetAttributesCount() const {
		return m_Attributes.size();
	}

	VertexAttribute VertexLayout::GetAttributeAt(int index) const {
		if (index < 0 || index >= m_Attributes.size()) {
			Log("Assert invalid index of vertex attribute\n");
		}

		for (auto& kvp : m_Attributes) {
			if (kvp.second.Index == index) {
				return kvp.second;
			}
		}

		Log("Assert invalid data in vertex layout\n");

		return VertexAttribute();
	}

	VertexAttribute VertexLayout::GetAttributeAt(VertexSemantic semantic) const {
		if (m_Attributes.find(semantic) == m_Attributes.end()) {
			Log("Assert semantic not found in vertex layout\n");
			return VertexAttribute();
		}

		return m_Attributes.at(semantic);
	}
}