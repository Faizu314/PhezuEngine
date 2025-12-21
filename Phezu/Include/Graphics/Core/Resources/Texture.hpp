#pragma once

namespace Phezu {

	enum class TextureWrapMode {
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFilteringMode {
		Point,
		Bilinear
	};

	struct TextureSettings {
		unsigned int Width;
		unsigned int Height;
		TextureWrapMode WrapMode;
		TextureFilteringMode FilteringMode;
	};

	class ITexture {
	public:
		virtual void Init(unsigned char *data, TextureSettings settings) = 0;
		virtual void Bind() = 0;
		virtual void Destroy() = 0;
	};
}