// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "glad/glad.h"

#include "Graphics/Core/Resources/VertexArray.hpp"

namespace Phezu {

	class GLVertexArray : public IVertexArray {
	public:
		void Init() override;
		void Bind() override;
		void Destroy() override;
	public:
		void LinkVertexBuffer(IVertexBuffer* vertexBuffer) override;
		void LinkIndexBuffer(IIndexBuffer* indexBuffer) override;
		void ApplyLayout(const VertexLayout* layout, const IShader* shader) override;
	private:
		GLuint m_Array = 0;
	};
}
