#pragma once

#include "Graphics/Core/Resources/VertexBuffer.hpp"
#include "Graphics/Core/Resources/IndexBuffer.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

	class IVertexArray {
	public:
		virtual void Init() = 0;
		virtual void Bind() = 0;
		virtual void Destroy() = 0;
	public:
		virtual void LinkVertexBuffer(IVertexBuffer* vertexBuffer, const VertexLayout& layout) = 0;
		virtual void LinkIndexBuffer(IIndexBuffer* indexBuffer) = 0;
	};
}