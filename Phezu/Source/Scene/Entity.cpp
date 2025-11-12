#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Components/ShapeData.hpp"
#include "Scene/Components/RenderData.hpp"
#include "Scene/Components/PhysicsData.hpp"
#include "Scene/Components/CameraData.hpp"
#include "Scene/Components/ScriptComponent.hpp"

#include "Logger.hpp"

namespace Phezu {
    
    uint64_t Entity::s_EntitiesCount = 0;
    
    Entity::Entity(Scene* scene) : m_Scene(scene), m_TransformData(this), m_Parent(nullptr), IsActive(true), Tag("Default") {
        m_EntityID = s_EntitiesCount;
        s_EntitiesCount++;
    }
    
    Entity::~Entity() {
        
    }

    ScriptComponent* Entity::GetScriptComponent(size_t index)
    {
        if (index < 0 || index >= m_ScriptComponents.size())
            return nullptr;

        return m_ScriptComponents[index];
    }

    DataComponent* Entity::AddDataComponent(ComponentType componentType) {
        if (HasDataComponent(componentType)) {
            Log("Native component of type %i already added to entity", componentType);
            return nullptr;
        }
        
        DataComponent* component;
        
        switch (componentType) {
            case ComponentType::Transform:
                Log("Trying to add Transform component to entity");
                return nullptr;
            case ComponentType::Shape:
                component = new ShapeData(this);
                break;
            case ComponentType::Render:
                component = new RenderData(this);
                break;
            case ComponentType::Physics:
                component = new PhysicsData(this);
                break;
            case ComponentType::Camera:
                component = new CameraData(this);
                break;
            default:
                return nullptr;
        }
        
        m_DataComponents.insert(std::make_pair(componentType, component));
        
        return component;
    }
    
    DataComponent* Entity::GetDataComponent(ComponentType componentType) {
        if (!HasDataComponent(componentType)) {
            Log("Native component of type %i does not exist on entity", componentType);
            return nullptr;
        }
        
        if (componentType == ComponentType::Transform)
            return &m_TransformData;
        
        return m_DataComponents.at(componentType);
    }
    
    void Entity::RemoveDataComponent(ComponentType componentType) {
        if (!HasDataComponent(componentType)) {
            Log("Trying to remove data component of type %i that does not exist", componentType);
            return;
        }
        
        delete m_DataComponents[componentType];
        
        m_DataComponents.erase(componentType);
    }
    
    bool Entity::HasDataComponent(ComponentType componentType) const {
        if (componentType == ComponentType::Transform)
            return true;
        
        return m_DataComponents.find(componentType) != m_DataComponents.end();
    }

    ScriptComponent* Entity::AddScriptComponent(const std::string& classFullname) {
        m_ScriptComponents.push_back(new ScriptComponent(this, classFullname));
        return m_ScriptComponents[m_ScriptComponents.size() - 1];
    }
    
    void Entity::RemoveScriptComponent(const std::string& classFullname) {
        for (size_t i = 0; i < m_ScriptComponents.size(); i++) {
            if (m_ScriptComponents[i]->GetScriptClassFullname() == classFullname) {
                m_ScriptComponents.erase(m_ScriptComponents.begin() + i);
                return;
            }
        }
    }

    bool Entity::HasScriptComponent(const std::string& classFullname) const {
        for (size_t i = 0; i < m_ScriptComponents.size(); i++) {
            if (m_ScriptComponents[i]->GetScriptClassFullname() == classFullname)
                return true;
        }

        return false;
    }

    TransformData* Entity::GetParent() const {
        if (m_Parent == nullptr)
            return nullptr;
        
        return dynamic_cast<TransformData*>(m_Parent->GetDataComponent(ComponentType::Transform));
    }

    Entity* Entity::GetChild(size_t childIndex) {
        if (childIndex >= m_Children.size())
            return nullptr;
        
        return m_Children[childIndex];
    }
    
    void Entity::Destroy() {
        m_Scene->DestroyEntity(m_EntityID);
    }
    
    void Entity::AddChild(Entity* child) {
        m_Children.push_back(child);
    }
    
    void Entity::RecalculateSubtreeTransforms() {
        m_TransformData.RecalculateLocalToWorld();
        
        for (auto child : m_Children)
            child->RecalculateSubtreeTransforms();
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
        RecalculateSubtreeTransforms();
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
