#pragma once

#include <cstdint>
#include <vector>

#include "glad/glad.h"
#include "Graphics/Core/Resources/Texture.hpp"

namespace Phezu {

	class GLTexture : public ITexture {
	public:
		void Init(unsigned char* data, unsigned int width, unsigned int height, SamplerDesc settings) override;
		void Bind(uint8_t unitIndex) override;
		void Destroy() override;
		ResourceHandle GetHandle() override;
	public:
		void Resize(unsigned char* data, unsigned int width, unsigned int height) override;
	public:
		std::vector<unsigned char> GetPixelData() override;
	private:
		GLuint m_Texture;
		size_t m_Size;
	};

}