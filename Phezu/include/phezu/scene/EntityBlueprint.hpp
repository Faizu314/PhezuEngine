#pragma once

#include <vector>
#include <string>
#include "Renderer.hpp"
#include "maths/Math.hpp"

namespace Phezu {
    
    class Entity;
    class ShapeData;
    
    class EntityBlueprint {
    public:
        EntityBlueprint();
        ~EntityBlueprint();
    public:
        EntityBlueprint& operator=(const EntityBlueprint& other) = delete;
    public:
        std::string TagOverride;
        Vector2 PositionOverride;
        Vector2 ScaleOverride;
        bool IsRenderable;
        bool IsCollidable;
        bool IsStatic;
        Vector2 VelocityOverride;
        Vector2 ShapePivotOverride;
        Vector2 ShapeSizeOverride;
        std::weak_ptr<Texture> TextureOverride;
        Color TintOverride;
        QuadUVs UVsOverride;
        std::string Comp;
    public:
        EntityBlueprint& CreateChildEntity();
        size_t GetChildCount() const;
        const EntityBlueprint* GetChild(size_t childIndex) const;
    private:
        EntityBlueprint(const EntityBlueprint& other);
        EntityBlueprint(std::unique_ptr<size_t[]> path, size_t pathSize);
    private:
        const bool m_IsRoot;
        const std::unique_ptr<size_t[]> m_Path;
        const size_t m_PathSize;
    private:
        std::vector<EntityBlueprint*> m_Children;
        
        friend class EntityTemplate;
    };
}
