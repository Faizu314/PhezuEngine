#pragma once

#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class Entity;
    
    class CameraData : public DataComponent {
    public:
        CameraData(Entity* entity, float size = 10.0) : DataComponent(entity), Size(size) {}
    public:
        float Size;
    };
}
