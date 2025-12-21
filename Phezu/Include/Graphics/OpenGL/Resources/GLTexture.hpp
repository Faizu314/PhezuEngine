#pragma once

#include "glad/glad.h"

#include "Graphics/Core/Resources/Texture.hpp"

namespace Phezu {

	class GLTexture : public ITexture {
	public:
		void Init(unsigned char* data, unsigned int width, unsigned int height, SamplerDesc settings) override;
		void Bind() override;
		void Destroy() override;
	private:
		GLuint m_Texture;
	};

}