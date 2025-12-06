#pragma once

#include "Graphics/Renderer/Renderer.hpp"

namespace Phezu {

    class Texture {
    private:
        Texture() = delete;

        friend class Renderer;
    };
}