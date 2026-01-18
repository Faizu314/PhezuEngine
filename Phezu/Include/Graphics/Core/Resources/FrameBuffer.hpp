#pragma once

namespace Phezu {

	class ITexture;

	class IFrameBuffer {
	public:
		virtual void Init() = 0;
		virtual void Bind() = 0;
		virtual void Destroy() = 0;
	public:
		virtual void AttachTexture(ITexture* texture) = 0;
	};

}