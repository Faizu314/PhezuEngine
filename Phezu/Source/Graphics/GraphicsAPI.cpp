#include "Graphics/GraphicsAPI.hpp"
#include "Graphics/OpenGL/OpenGLAPI.hpp"

namespace Phezu {

	IGraphicsAPI* CreateGraphicsAPI() {
		return new OpenGLAPI();
	}
}