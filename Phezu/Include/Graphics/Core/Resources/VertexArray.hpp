#pragma once

namespace Phezu {

	class IVertexBuffer;
	class IIndexBuffer;
	class VertexLayout;
	class IShader;

	class IVertexArray {
	public:
		virtual void Init() = 0;
		virtual void Bind() = 0;
		virtual void Destroy() = 0;
	public:
		virtual void LinkVertexBuffer(IVertexBuffer* vertexBuffer) = 0;
		virtual void LinkIndexBuffer(IIndexBuffer* indexBuffer) = 0;
		virtual void ApplyLayout(const VertexLayout* layout, const IShader* shader) = 0;
	};
}