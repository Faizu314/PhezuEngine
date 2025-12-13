#include "Graphics/OpenGL/Resources/GLVertexArray.hpp"

namespace Phezu {

	void GLVertexArray::Init() {
		glGenVertexArrays(1, &m_Array);
	}

	void GLVertexArray::Bind() {
		glBindVertexArray(m_Array);
	}

	void GLVertexArray::Destroy() {
		if (m_Array != 0)
			glDeleteVertexArrays(1, &m_Array);
		m_Array = 0;
	}

	void GLVertexArray::LinkVertexBuffer(IVertexBuffer* vertexBuffer, const VertexLayout& layout) {
		Bind();
		vertexBuffer->Bind();

		ApplyVertexLayout(layout);
	}

	void GLVertexArray::LinkIndexBuffer(IIndexBuffer* indexBuffer) {
		Bind();
		indexBuffer->Bind();
	}

	void GLVertexArray::ApplyVertexLayout(const VertexLayout& layout) {

	}
}
