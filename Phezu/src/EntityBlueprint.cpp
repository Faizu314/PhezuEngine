#include "scene/EntityBlueprint.hpp"
#include "scene/components/BehaviourComponentPrefab.hpp"

namespace Phezu {
    
    EntityBlueprint::EntityBlueprint()
    : m_IsRoot(true), m_PathSize(0), ScaleOverride(Vector2(1, 1)), ShapeSizeOverride(Vector2(10, 10)), TintOverride(Color::White), TagOverride("Default") {}
    
    EntityBlueprint::EntityBlueprint(std::unique_ptr<size_t[]> path, size_t pathSize)
    : m_IsRoot(false), m_Path(std::move(path)), m_PathSize(pathSize), ScaleOverride(Vector2(1, 1)), ShapeSizeOverride(Vector2(10, 10)), TintOverride(Color::White) {}
    
    EntityBlueprint::~EntityBlueprint() {
        for (EntityBlueprint* child : m_Children)
            delete child;
    }

    EntityBlueprint::EntityBlueprint(const EntityBlueprint& other) :
        TagOverride(other.TagOverride),
        PositionOverride(other.PositionOverride),
        ScaleOverride(other.ScaleOverride),
        IsRenderable(other.IsRenderable),
        IsCollidable(other.IsCollidable),
        IsStatic(other.IsStatic),
        VelocityOverride(other.VelocityOverride),
        ShapePivotOverride(other.ShapePivotOverride),
        ShapeSizeOverride(other.ShapeSizeOverride),
        TextureOverride(other.TextureOverride),
        TintOverride(other.TintOverride),
        UVsOverride(other.UVsOverride),
        m_IsRoot(other.m_IsRoot),
        m_Path(std::make_unique<size_t[]>(other.m_PathSize)),
        m_PathSize(other.m_PathSize)
    {
        if (!other.m_IsRoot) {
            //TODO: Logging
        }
        
        for (size_t i = 0; i < m_PathSize; i++)
            m_Path[i] = other.m_Path[i];
        
        for (size_t i = 0; i < other.m_BehaviourComponents.size(); i++) {
            m_BehaviourComponents.push_back(other.m_BehaviourComponents[i]->Clone());
        }
        
        for (size_t i = 0; i < other.m_Children.size(); i++) {
            m_Children.emplace_back(new EntityBlueprint(*other.m_Children[i]));
        }
    }
    
    EntityBlueprint& EntityBlueprint::CreateChildEntity() {
        std::unique_ptr<size_t[]> path = std::make_unique<size_t[]>(m_PathSize + 1);
        for (int i = 0; i < m_PathSize; i++)
            path[i] = m_Path[i];
        path[m_PathSize] = m_Children.size();
        
        EntityBlueprint* child = new EntityBlueprint(std::move(path), m_PathSize + 1);
        m_Children.push_back(child);
        
        return *child;
    }
    
    size_t EntityBlueprint::GetChildCount() const {
        return m_Children.size();
    }
    
    const EntityBlueprint* EntityBlueprint::GetChild(size_t childIndex) const {
        if (childIndex >= m_Children.size())
            return nullptr;
        
        return m_Children[childIndex];
    }
    
    std::weak_ptr<BehaviourComponentPrefabBase> EntityBlueprint::GetComponentPrefab(size_t index) const {
        if (index >= m_BehaviourComponents.size()) {
            //TODO: copy and paste the logging class
            return std::weak_ptr<BehaviourComponentPrefabBase>();
        }
        return m_BehaviourComponents[index];
    }
    
    size_t EntityBlueprint::GetComponentPrefabsCount() const {
        return m_BehaviourComponents.size();
    }
}
