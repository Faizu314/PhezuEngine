#pragma once

#include <unordered_map>
#include <string>

#include "glad/glad.h"

#include "Graphics/Core/Resources/Shader.hpp"

namespace Phezu {
	
	class GLShader : public IShader {
	public:
		void Init(const std::string& vert, const std::string& frag) override;
		void Bind() override;
		void Destroy() override;
	public:
		void SetVec4(const std::string& uniformName, Color color) override;
	private:
		GLuint m_ShaderProgram = 0;
		std::unordered_map<std::string, GLuint> m_UniformLocations;
	};
}
