#pragma once

#include "Core/Platform.hpp"

namespace Phezu {

	class IGraphicsAPI {
	public:
		virtual int Init(IPlatform* platform) = 0;
	};

	IGraphicsAPI* CreateGraphicsAPI();
}