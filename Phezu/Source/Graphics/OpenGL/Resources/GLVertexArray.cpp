#include "Core/Platform.hpp"
#include "Graphics/Core/GraphicsTypes.hpp"
#include "Graphics/OpenGL/Resources/GLVertexArray.hpp"
#include "Graphics/Core/Resources/VertexBuffer.hpp"
#include "Graphics/Core/Resources/IndexBuffer.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"

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

	void GLVertexArray::LinkVertexBuffer(IVertexBuffer* vertexBuffer) {
		Bind();
		vertexBuffer->Bind();
	}

	void GLVertexArray::LinkIndexBuffer(IIndexBuffer* indexBuffer) {
		Bind();
		indexBuffer->Bind();
	}

	void GLVertexArray::ApplyLayout(const VertexLayout* layout, const IShader* shader) {
		GLsizei stride = 0;
		uint64_t offset = 0;

		for (int i = 0; i < layout->GetAttributesCount(); i++) {
			stride += GetVertexAttributeSize(layout->GetAttributeAt(i));
		}

		for (int i = 0; i < layout->GetAttributesCount(); i++) {
			VertexAttribute attrib = layout->GetAttributeAt(i);
			GLint count = GetVertexAttributeCount(attrib.AttributeCount);
			GLboolean normalized = attrib.Normalized ? GL_TRUE : GL_FALSE;

			switch (attrib.AttributeType) {
				case VertexAttributeType::Float:
					glVertexAttribPointer(i, count, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
					break;

				case VertexAttributeType::Int:
					glVertexAttribIPointer(i, count, GL_INT, stride, (void*)(offset));
					break;

				case VertexAttributeType::UInt:
					glVertexAttribIPointer(i, count, GL_UNSIGNED_INT, stride, (void*)(offset));
					break;

				case VertexAttributeType::Byte:
					glVertexAttribPointer(i, count, GL_BYTE, normalized, stride, (void*)(offset));
					break;

				case VertexAttributeType::UByte:
					glVertexAttribPointer(i, count, GL_UNSIGNED_BYTE, normalized, stride, (void*)(offset));
					break;

				default: {
					Log("Should Assert Here\n");
				}
			}

			glEnableVertexAttribArray(i);

			offset += GetVertexAttributeSize(attrib);
		}
	}
}
