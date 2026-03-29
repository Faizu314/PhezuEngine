#include "Maths/Objects/Vector3.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/TransformData.hpp"

namespace Phezu {

    TransformData::TransformData(Entity* entity, Vector2 localPosition, float localRotation, Vector2 localScale) 
        : DataComponent(entity), m_LocalPosition(localPosition), m_LocalRotation(localRotation), m_LocalScale(localScale)
    {
        RecalculateLocalToWorld();
    }
    
    void TransformData::SetLocalPosition(Vector2 position) {
        m_LocalPosition = position;
        m_IsDirty = true;
    }

    void TransformData::SetWorldPosition(Vector2 position) {
        TransformData* parent = m_Entity->GetParent();
        
        if (parent == nullptr)
            m_LocalPosition = position;
        else
            m_LocalPosition = parent->WorldToLocalPoint(position);
        
        m_IsDirty = true;
    }
    
    Vector2 TransformData::GetWorldPosition() const {
        TransformData* parent = m_Entity->GetParent();
        
        if (parent == nullptr)
            return m_LocalPosition;
        
        return parent->LocalToWorldPoint(m_LocalPosition);
    }
    
    void TransformData::SetLocalScale(Vector2 scale) {
        m_LocalScale = scale;
        m_IsDirty = true;
    }

    void TransformData::SetLocalRotation(float rotation) {
        m_LocalRotation = rotation;
        m_IsDirty = true;
    }
    
    Vector2 TransformData::LocalToWorldPoint(Vector2 localPoint) const {
        Vector3 point3(localPoint.X(), localPoint.Y(), 1);

        return m_LocalToWorld * point3;
    }
    
    Vector2 TransformData::WorldToLocalPoint(Vector2 worldPoint) const {
        Vector3 point3(worldPoint.X(), worldPoint.Y(), 1);

        return m_WorldToLocal * point3;
    }
    
    void TransformData::RecalculateLocalToWorld() {
        Mat3x3 localTransform;
        
        float Sx = m_LocalScale.X();
        float Sy = m_LocalScale.Y();
        float Tx = m_LocalPosition.X();
        float Ty = m_LocalPosition.Y();
        float S = glm::sin(m_LocalRotation);
        float C = glm::cos(m_LocalRotation);
        localTransform.Set(0, 0, Sx * C);
        localTransform.Set(1, 1, Sy * C);
        localTransform.Set(1, 0, -S * Sx);
        localTransform.Set(0, 1, S * Sy);
        localTransform.Set(2, 0, Tx);
        localTransform.Set(2, 1, Ty);
        
        if (m_Entity->GetParent() == nullptr)
            m_LocalToWorld = localTransform;
        else
            m_LocalToWorld = m_Entity->GetParent()->m_LocalToWorld * localTransform;
        
        m_WorldToLocal = m_LocalToWorld.Inversed();
        
        m_IsDirty = false;
    }
}
