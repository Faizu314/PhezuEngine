#include "Core/Platform.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/Core/Resources/VertexBuffer.hpp"
#include "Graphics/Core/Resources/IndexBuffer.hpp"
#include "Graphics/Core/Resources/VertexArray.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Data/Mesh.hpp"

namespace Phezu {

	Mesh::~Mesh() {
		Destroy();
	}

	void Mesh::Init(IGraphicsAPI* api) {
		m_Api = api;
	}

	void Mesh::Destroy() {
		if (m_VertexBuffer != nullptr) {
			m_VertexBuffer->Destroy();
			m_VertexBuffer = nullptr;
		}
		if (m_IndexBuffer != nullptr) {
			m_IndexBuffer->Destroy();
			m_IndexBuffer = nullptr;
		}
		if (m_VertexArray != nullptr) {
			m_VertexArray->Destroy();
			m_VertexArray = nullptr;
		}
	}

	void Mesh::AttachVertexBuffer(const void* vertices, size_t bufferSize, BufferType bufferType, const VertexLayout& layout) {
		if (m_Api == nullptr || m_VertexBuffer != nullptr) {
			Log("Should assert here\n");
			return;
		}

		m_Data.AttachedVertices = vertices;
		m_Data.VertexBufferSize = bufferSize;
		m_Data.VertexBufferType = bufferType;
		m_Layout = layout;
	}

	void Mesh::AttachIndexBuffer(const unsigned int* indices, size_t bufferSize, BufferType bufferType) {
		if (m_Api == nullptr || m_IndexBuffer != nullptr) {
			Log("Should assert here\n");
			return;
		}

		m_Data.AttachedIndices = indices;
		m_Data.IndexBufferSize = bufferSize;
		m_Data.IndexBufferType = bufferType;
	}

	void Mesh::UploadMeshData() {
		if (m_Api == nullptr) {
			Log("Should assert here\n");
			return;
		}

		m_VertexBuffer = m_Api->CreateVertexBuffer(m_Data.AttachedVertices, m_Data.VertexBufferSize, m_Data.VertexBufferType);
		m_IndexBuffer = m_Api->CreateIndexBuffer(m_Data.AttachedIndices, m_Data.IndexBufferSize, m_Data.IndexBufferType);
		m_VertexArray = m_Api->CreateVertexArray();

		m_VertexArray->LinkVertexBuffer(m_VertexBuffer);
		m_VertexArray->LinkIndexBuffer(m_IndexBuffer);
	}

	void Mesh::Bind(const IShader* shader) const {
		if (m_Api == nullptr || m_VertexBuffer == nullptr || m_IndexBuffer == nullptr || m_VertexArray == nullptr) {
			Log("Should assert here\n");
			return;
		}

		m_VertexArray->Bind();
		m_VertexArray->ApplyLayout(&m_Layout, shader);
	}

	unsigned int Mesh::GetIndicesCount() const {
		if (m_IndexBuffer == nullptr) {
			Log("Should assert here\n");
			return 0;
		}

		return m_IndexBuffer->GetIndicesCount();
	}
}