#pragma once

#include <string>

#include "Core/Types/Color.hpp"

namespace Phezu {

	class IShader {
	public:
		virtual void Init(const std::string& vert, const std::string& frag) = 0;
		virtual void Bind() = 0;
		virtual void Destroy() = 0;
	public:
		virtual void SetVec4(const std::string& uniformName, Color color) = 0;
	};
}