#pragma once

#include <vector>
#include <string>
#include "Renderer.hpp"
#include "maths/Math.hpp"

namespace Phezu {
    
    class Entity;
    class ShapeData;
    class BehaviourComponentPrefabBase;
    
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
    public:
        template<typename T>
        std::weak_ptr<T> AddComponentPrefab() {
            if (!std::is_base_of<BehaviourComponentPrefabBase, T>::value) {
                //TODO: copy and paste the logging class
                return std::weak_ptr<T>();
            }
            
            uint8_t componentID = static_cast<uint8_t>(m_BehaviourComponents.size());
            
            std::unique_ptr<size_t[]> path = std::make_unique<size_t[]>(m_PathSize);
            for (int i = 0; i < m_PathSize; i++)
                path[i] = m_Path[i];
            
            std::shared_ptr<T> component = std::make_shared<T>(std::move(path), m_PathSize, componentID);
            
            m_BehaviourComponents.push_back(std::static_pointer_cast<BehaviourComponentPrefabBase>(component));
            
            return component;
        }
    public:
        size_t GetComponentPrefabsCount() const;
        std::weak_ptr<BehaviourComponentPrefabBase> GetComponentPrefab(size_t index) const;
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
        std::vector<std::shared_ptr<BehaviourComponentPrefabBase>> m_BehaviourComponents;
        std::vector<EntityBlueprint*> m_Children;
        
        friend class EntityTemplate;
    };
}
