#pragma once

#include "Core/Types/Color.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        RenderData(Entity* entity, void* texture = nullptr, Color tint = Color::White) : DataComponent(entity), Sprite(texture), Tint(tint) {}
    public:
        //Texture
        void* Sprite;
        Color Tint;
    };
}
