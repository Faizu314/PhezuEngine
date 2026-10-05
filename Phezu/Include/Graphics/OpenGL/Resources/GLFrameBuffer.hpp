// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#if defined(PZ_PLATFORM_WINDOWS)
	#include "glad/glad.h"
#elif defined(PZ_PLATFORM_WEB)
	#include <GLES3/gl3.h>
#endif

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