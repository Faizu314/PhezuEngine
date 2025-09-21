#include "scene/Entity.hpp"
#include "scene/Scene.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/ScriptComponent.hpp"

namespace Phezu {
    
    uint64_t Entity::s_EntitiesCount = 0;
    
    Entity::Entity(Scene* scene) : m_Scene(scene), m_TransformData(this), m_ShapeData(nullptr), m_RenderData(nullptr), m_PhysicsData(nullptr), m_Parent(nullptr), m_IsActive(true), m_Tag("Default") {
        m_EntityID = s_EntitiesCount;
        s_EntitiesCount++;
    }
    
    Entity::~Entity() {
        if (m_ShapeData != nullptr)
            delete m_ShapeData;
        if (m_RenderData != nullptr)
            delete m_RenderData;
        if (m_PhysicsData != nullptr)
            delete m_PhysicsData;
    }
    

    TransformData* Entity::GetTransformData() {
        return &m_TransformData;
    }

    ScriptComponent* Entity::GetScriptComponent(size_t index)
    {
        if (index < 0 || index >= m_ScriptComponents.size())
            return nullptr;

        return &m_ScriptComponents[index];
    }

    ShapeData* Entity::AddShapeData() {
        m_ShapeData = new ShapeData(this);
        return m_ShapeData;
    }

    RenderData* Entity::AddRenderData(Color tint) {
        m_RenderData = new RenderData(this, tint);
        return m_RenderData;
    }

    PhysicsData* Entity::AddPhysicsData(bool isStatic) {
        m_PhysicsData = new PhysicsData(this, isStatic);
        return m_PhysicsData;
    }

    ScriptComponent* Entity::AddScriptComponent(const std::string& classFullname) {
        m_ScriptComponents.emplace_back(this, classFullname);
        return &m_ScriptComponents[m_ScriptComponents.size() - 1];
    }

    bool Entity::HasScriptComponent(const std::string& classFullname) {
        for (size_t i = 0; i < m_ScriptComponents.size(); i++) {
            if (m_ScriptComponents[i].GetScriptClassFullname() == classFullname)
                return true;
        }

        return false;
    }

    TransformData* Entity::GetParent() const {
        if (m_Parent == nullptr)
            return nullptr;
        
        return m_Parent->GetTransformData();
    }

    Entity* Entity::GetChild(size_t childIndex) {
        if (childIndex >= m_Children.size())
            return nullptr;
        
        return m_Children[childIndex];
    }
    
    void Entity::AddChild(Entity* child) {
        m_Children.push_back(child);
    }
    
    void Entity::RecalculateSubtreeTransformations() {
        m_TransformData.RecalculateLocalToWorld();
        
        for (auto child : m_Children)
            child->RecalculateSubtreeTransformations();
    }
    
    void Entity::SetParent(Entity* parent) {
        if (parent == nullptr) {
            RemoveParent();
            return;
        }
        if (parent == m_Parent)
            return;
        
        // Check if parenting would be cyclic
        Entity* currParent = parent;
        while (currParent != nullptr) {
            if (currParent == this)
                return;
            
            currParent = currParent->m_Parent;
        }
        
        m_Parent->OnChildRemoved(this);
        m_Parent = parent;
        m_Parent->AddChild(this);
        RecalculateSubtreeTransformations();
    }
    
    void Entity::RemoveParent() {
        if (m_Parent == nullptr)
            return;
        
        m_Parent->OnChildRemoved(this);
        m_Parent = nullptr;
    }
    
    void Entity::OnDestroyed() {
        if (m_Parent != nullptr)
            m_Parent->OnChildRemoved(this);
    }
    
    void Entity::OnChildRemoved(const Entity* removedChild) {
        m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), removedChild));
    }
}
