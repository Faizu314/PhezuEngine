#pragma once

#include "Graphics/Renderer/Shader.hpp"

namespace Phezu {
	
	class GLShader : public IShader {
	public:
		void Init(const std::string& vert, const std::string& frag);
		void Bind();
	private:
		unsigned int m_ShaderProgram = 0;
	};
}