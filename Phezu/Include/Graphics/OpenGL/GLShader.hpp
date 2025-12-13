#pragma once

#include <unordered_map>
#include <string>

#include "Graphics/Renderer/Shader.hpp"
#include "glad/glad.h"

namespace Phezu {
	
	class GLShader : public IShader {
	public:
		void Init(const std::string& vert, const std::string& frag) override;
		void Bind() override;
	public:
		void SetVec4(const std::string& uniformName, Color color) override;
	private:
		GLuint m_ShaderProgram = 0;
		std::unordered_map<std::string, GLuint> m_UniformLocations;
	};
}