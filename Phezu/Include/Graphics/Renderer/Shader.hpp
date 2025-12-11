#pragma once

#include <string>

namespace Phezu {

	class IShader {
	public:
		virtual void Init(const std::string& vert, const std::string& frag) = 0;
		virtual void Bind() = 0;
	};
}