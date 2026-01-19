// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class Entity;
    
    class CameraData : public DataComponent {
    public:
        CameraData(Entity* entity, float size = 10.0) : DataComponent(entity), Size(size) {}
    public:
        float Size;
    };
}
