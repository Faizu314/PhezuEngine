#pragma once

#include "Graphics/Core/GraphicsTypes.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

	class IGraphicsAPI;
	class IVertexBuffer;
	class IIndexBuffer;
	class IVertexArray;
	class IShader;

	struct AttachedDataRef {
		const void* AttachedVertices = nullptr;
		size_t VertexBufferSize = 0;
		BufferType VertexBufferType;

		const unsigned int* AttachedIndices = nullptr;
		size_t IndexBufferSize = 0;
		BufferType IndexBufferType;
	};

	class Mesh {
	public:
		Mesh() = default;
		~Mesh();
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
	public:
		void Init(IGraphicsAPI* api);
		void Destroy();
	public:
		void AttachVertexBuffer(const void* vertices, size_t bufferSize, BufferType bufferType, const VertexLayout& layout);
		void AttachIndexBuffer(const unsigned int* indices, size_t bufferSize, BufferType bufferType);
	public:
		void UploadMeshData();
		void Bind(const IShader* shader);
	private:
		IGraphicsAPI* m_Api = nullptr;
		AttachedDataRef m_Data;
	private:
		VertexLayout m_Layout;
		IVertexBuffer* m_VertexBuffer = nullptr;
		IIndexBuffer* m_IndexBuffer = nullptr;
		IVertexArray* m_VertexArray = nullptr;
	};

}