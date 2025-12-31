#pragma once

#include "Core/Types/Types.hpp"

namespace Phezu {

	class ITexture {
	public:
		virtual void Init(unsigned char *data, unsigned int width, unsigned int height, SamplerDesc settings) = 0;
		virtual void Bind(uint8_t unitIndex) = 0;
		virtual void Destroy() = 0;
	};
}