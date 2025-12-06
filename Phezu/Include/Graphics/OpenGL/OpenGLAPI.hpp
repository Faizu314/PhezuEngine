#pragma once

#include "Graphics/GraphicsAPI.hpp"

namespace Phezu {

	class OpenGLAPI : public IGraphicsAPI {
	public:
		int Init(IPlatform* platform) override;
	};
}