#include "Core/Platform.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/OpenGL/Resources/GLFrameBuffer.hpp"

namespace Phezu {

	void GLFrameBuffer::Init() {
		glGenFramebuffers(1, &m_FrameBuffer);
	}

	void GLFrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	}

	void GLFrameBuffer::Destroy() {
		glDeleteFramebuffers(1, &m_FrameBuffer);
	}

	void GLFrameBuffer::AttachTexture(ITexture* texture) {
		GLuint texturePtr = static_cast<GLuint>(texture->GetHandle().Ptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texturePtr, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Log("Should assert here, Framebuffer is not complete!\n");
	}

}