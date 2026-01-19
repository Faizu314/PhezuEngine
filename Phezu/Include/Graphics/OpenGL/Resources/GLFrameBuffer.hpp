// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "glad/glad.h"

#include "Graphics/Core/Resources/FrameBuffer.hpp"

namespace Phezu {

	class GLFrameBuffer : public IFrameBuffer {
	public:
		void Init() override;
		void Bind() override;
		void Destroy() override;
	public:
		void AttachTexture(ITexture* texture) override;
	private:
		GLuint m_FrameBuffer;
	};

}