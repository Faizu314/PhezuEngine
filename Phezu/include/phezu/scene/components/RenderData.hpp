#pragma once

#include "Renderer.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        RenderData(Entity* entity) : DataComponent(entity), Tint(Color::White) {}
        RenderData(Entity* entity, Color tint) : DataComponent(entity), Tint(tint) {}
    public:
        RenderData(Entity* entity, Texture* texture) : DataComponent(entity), Sprite(texture), Tint(Color::White) {}
        RenderData(Entity* entity, Texture* texture, Color tint) : DataComponent(entity), Sprite(texture), Tint(tint) {}
        RenderData(Entity* entity, Texture* texture, Rect uvs) : DataComponent(entity), Sprite(texture), SourceRect(uvs), Tint(Color::White) {}
        RenderData(Entity* entity, Texture* texture, Color tint, Rect uvs) : DataComponent(entity), Sprite(texture), Tint(tint), SourceRect(uvs) {}
    public:
        Texture* Sprite;
        Rect SourceRect;
        Color Tint;
    };
}
