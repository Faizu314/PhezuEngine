#pragma once

#include "glad/glad.h"
#include "Graphics/Core/GraphicsTypes.hpp"
#include "Graphics/Core/Resources/VertexArray.hpp"

namespace Phezu {

	class GLVertexArray : public IVertexArray {
	public:
		void Init() override;
		void Bind() override;
		void Destroy() override;
	public:
		void LinkVertexBuffer(IVertexBuffer* vertexBuffer, const VertexLayout& layout) override;
		void LinkIndexBuffer(IIndexBuffer* indexBuffer) override;
	private:
		void ApplyVertexLayout(const VertexLayout& layout);
	private:
		GLuint m_Array = 0;
	};
}
