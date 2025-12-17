#pragma once

#include <unordered_map>

#include "Graphics/Core/GraphicsTypes.hpp"

namespace Phezu {

	class VertexLayout {
	public:
		VertexLayout(std::initializer_list<VertexAttribute> list);
	public:
		size_t GetAttributesCount() const;
		bool HasSemantic(VertexSemantic semantic) const;
		VertexAttribute GetAttributeAt(VertexSemantic semantic) const;
		unsigned int GetStride() const { return m_Stride; }
	private:
		void EvaluateOffsetAndStride();
	private:
		unsigned int m_Stride;
		std::unordered_map<VertexSemantic, VertexAttribute> m_Attributes;
	};

	unsigned int GetVertexAttributeSize(const VertexAttribute& attribute);
	unsigned int GetVertexAttributeCount(VertexAttributeCount countEnum);
}