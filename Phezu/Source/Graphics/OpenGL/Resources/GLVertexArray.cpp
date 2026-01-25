#include "Core/Defs/Assert.hpp"
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
		for (GLuint i = 0; i < layout->GetAttributesCount(); i++)
			glDisableVertexAttribArray(i);

		GLsizei stride = static_cast<GLsizei>(layout->GetStride());
		std::vector<VertexSemantic> requiredSemantics = shader->GetRequiredSemantics();

		for (size_t i = 0; i < requiredSemantics.size(); i++) {
			VertexSemantic semantic = requiredSemantics[i];
			GLuint location = static_cast<GLuint>(shader->GetSemanticLocation(semantic));

			if (!layout->HasSemantic(semantic)) {
				switch (semantic) {
					case VertexSemantic::Position:
					case VertexSemantic::Color:
					case VertexSemantic::TexCoord0:
					case VertexSemantic::TexCoord1:
					case VertexSemantic::TexCoord2:
					case VertexSemantic::TexCoord3:
					case VertexSemantic::Custom0:
					case VertexSemantic::Custom1:
					case VertexSemantic::Custom2:
					case VertexSemantic::Custom3:
						glVertexAttrib4f(location, 0.0f, 0.0f, 0.0f, 0.0f); break;
					case VertexSemantic::Normal:
						glVertexAttrib3f(location, 0.0f, 0.0f, 0.0f); break;
					default: {
						PZ_ASSERT(false, "Unknown VertexSemantic.\n")
					}
				}
				
				continue;
			}

			VertexAttribute attrib = layout->GetAttributeAt(semantic);
			GLint count = static_cast<GLint>(GetVertexAttributeCount(attrib.AttributeCount));

			switch (attrib.AttributeType) {
				case VertexAttributeType::Float:
					glVertexAttribPointer(location, count, GL_FLOAT, GL_FALSE, stride, (void*)(attrib.Offset)); break;
				case VertexAttributeType::Int:
					glVertexAttribIPointer(location, count, GL_INT, stride, (void*)(attrib.Offset)); break;
				case VertexAttributeType::UInt:
					glVertexAttribIPointer(location, count, GL_UNSIGNED_INT, stride, (void*)(attrib.Offset)); break;
				case VertexAttributeType::Byte:
					glVertexAttribPointer(location, count, GL_BYTE, attrib.Normalized, stride, (void*)(attrib.Offset)); break;
				case VertexAttributeType::UByte:
					glVertexAttribPointer(location, count, GL_UNSIGNED_BYTE, attrib.Normalized, stride, (void*)(attrib.Offset)); break;
				default: {
					PZ_ASSERT(false, "Unknown VertexAttributeType.\n")
				}
			}

			glEnableVertexAttribArray(location);
		}
	}
}
