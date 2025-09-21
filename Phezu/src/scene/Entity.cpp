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
    
    uint64_t Entity::GetEntityID() const {
        return m_EntityID;
    }
    
    
    void Entity::SetActive(bool isActive) {
        m_IsActive = isActive;
    }

    bool Entity::GetActive() const {
        return m_IsActive;
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

    size_t Entity::GetChildCount() {
        return m_Children.size();
    }

    Entity* Entity::GetChild(size_t childIndex) {
        if (childIndex >= m_Children.size())
            return nullptr;
        
        return m_Children[childIndex];
    }
    
    bool Entity::IsDirty() {
        return m_TransformData.GetIsDirty();
    }
    
    void Entity::AddChild(Entity* child) {
        m_Children.push_back(child);
    }
    
    void Entity::RecalculateSubtreeTransformations() {
        m_TransformData.RecalculateLocalToWorld();
        
        for (auto child_ : m_Children) {
            auto child = child_;
            child->RecalculateSubtreeTransformations();
        }
    }
    
    void Entity::SetParent(Entity* parent) {
        if (parent == this)
            return;
        
        SetParentInternal(m_Scene->GetEntity(m_EntityID), parent);
    }
    
    //TODO: I need an internal AddChild(Entity* other) function. Don't need to use scene to get me a smart pointer of this
    
    void SetParentInternal(Entity* _this, Entity* parent) {
            _this->m_Parent = parent;
            parent->AddChild(_this);
            _this->RecalculateSubtreeTransformations();
    }
    
    void Entity::RemoveParent() {
        //TODO: 
    }
    
    void Entity::OnDestroyed() {
        if (m_Parent != nullptr)
            m_Parent->OnChildDestroyed();
    }
    
    void Entity::OnChildDestroyed() {
        //TODO: 
        // have to fix this function
    }
}
