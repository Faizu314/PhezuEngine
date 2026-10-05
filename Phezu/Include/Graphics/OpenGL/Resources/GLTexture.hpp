// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdint>
#include <vector>

#if defined(PZ_PLATFORM_WINDOWS)
	#include "glad/glad.h"
#elif defined(PZ_PLATFORM_WEB)
	#include <GLES3/gl3.h>
#endif

#include "Graphics/Core/Resources/Texture.hpp"

namespace Phezu {

	class GLTexture : public ITexture {
	public:
		void Init(unsigned char* data, unsigned int width, unsigned int height, SamplerDesc settings) override;
		void Bind(uint8_t unitIndex) override;
		void Destroy() override;
		ResourceNativePtr GetNativePtr() override;
	public:
		void Resize(unsigned char* data, unsigned int width, unsigned int height) override;
	public:
		std::vector<unsigned char> GetPixelData() override;
	private:
		GLuint m_Texture;
		size_t m_Size;
	};

}