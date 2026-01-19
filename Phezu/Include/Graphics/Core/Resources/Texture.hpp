// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <vector>
#include <cstdint>

#include "Graphics/Core/GraphicsTypes.hpp"
#include "Core/Types/Types.hpp"

namespace Phezu {

	class ITexture {
	public:
		virtual void Init(unsigned char *data, unsigned int width, unsigned int height, SamplerDesc settings) = 0;
		virtual void Bind(uint8_t unitIndex) = 0;
		virtual void Destroy() = 0;
		virtual ResourceHandle GetHandle() = 0;
	public:
		virtual void Resize(unsigned char* data, unsigned int width, unsigned int height) = 0;
	public:
		virtual std::vector<unsigned char> GetPixelData() = 0;
	};
}