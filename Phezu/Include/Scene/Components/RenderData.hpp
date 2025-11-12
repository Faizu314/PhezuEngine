#pragma once

#include "Renderer.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        RenderData(Entity* entity, Texture* texture = nullptr, Color tint = Color::White) : DataComponent(entity), Sprite(texture), Tint(tint) {}
    public:
        Texture* Sprite;
        Rect SourceRect;
        Color Tint;
    };
}
