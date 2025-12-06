#pragma once

#include "glad/glad.h"

#include "Graphics/OpenGL/OpenGLAPI.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	int OpenGLAPI::Init(IPlatform* platform) {
		int error = gladLoadGLLoader((GLADloadproc) platform->GetOpenGLFunctionLoader());

		if (error < 0) {
			Log("Failed to initialize GLAD with error code: %i\n", error);

			return error;
		}

		return 0;
	}
}