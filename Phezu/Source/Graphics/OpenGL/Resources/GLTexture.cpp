#include "Core/Platform.hpp"
#include "Graphics/OpenGL/Resources/GLTexture.hpp"

namespace Phezu {

	GLint GetGLWrapMode(TextureWrapMode wrapMode) {
		switch (wrapMode) {
			case TextureWrapMode::Repeat:
				return GL_REPEAT;
			case TextureWrapMode::MirroredRepeat:
				return GL_MIRRORED_REPEAT;
			case TextureWrapMode::ClampToEdge:
				return GL_CLAMP_TO_EDGE;
			case TextureWrapMode::ClampToBorder:
				return GL_CLAMP_TO_BORDER;
			default: {
				Log("Should assert here\n");
				return 0;
			}
		}
	}

	GLint GetGLFilterMode(TextureFilteringMode filteringMode) {
		switch (filteringMode) {
			case TextureFilteringMode::Point:
				return GL_NEAREST;
			case TextureFilteringMode::Bilinear:
				return GL_LINEAR;
			default: {
				Log("Should assert here\n");
				return 0;
			}
		}
	}


	void Phezu::GLTexture::Init(unsigned char* data, unsigned int width, unsigned int height, SamplerDesc settings) {
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		GLint wrapMode = GetGLWrapMode(settings.WrapMode);
		GLint filteringMode = GetGLFilterMode(settings.FilteringMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringMode);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture::Bind(uint8_t unitIndex) {
		glActiveTexture(GL_TEXTURE0 + unitIndex);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	}

	void GLTexture::Resize(unsigned char* data, unsigned int width, unsigned int height) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture::Destroy() {
		glDeleteTextures(1, &m_Texture);
	}

}