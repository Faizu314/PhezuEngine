#pragma once

#include <unordered_map>

#include "Graphics/Core/GraphicsTypes.hpp"

namespace Phezu {

	class VertexLayout {
	public:
		VertexLayout() = default;
	public:
		void Push(VertexAttribute attrib);
		void ComputeLayout();
	public:
		size_t GetAttributesCount() const;
		bool HasSemantic(VertexSemantic semantic) const;
		VertexAttribute GetAttributeAt(VertexSemantic semantic) const;
		unsigned int GetStride() const { return m_Stride; }
	private:
		void EvaluateOffsetAndStride();
	private:
		unsigned int m_Stride = 0;
		std::unordered_map<VertexSemantic, VertexAttribute> m_Attributes;
	};
}