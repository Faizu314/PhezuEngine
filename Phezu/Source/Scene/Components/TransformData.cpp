#include "Maths/Objects/Vector3.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/TransformData.hpp"

namespace Phezu {

    TransformData::TransformData(Entity* entity, Vector2 localPosition, Vector2 localScale) : DataComponent(entity), m_LocalPosition(localPosition), m_LocalScale(localScale) {
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
        float Px = m_LocalPosition.X();
        float Py = m_LocalPosition.Y();
        localTransform.Set(0, 0, Sx);
        localTransform.Set(1, 1, Sy);
        localTransform.Set(2, 0, Px);
        localTransform.Set(2, 1, Py);
        
        if (m_Entity->GetParent() == nullptr)
            m_LocalToWorld = localTransform;
        else
            m_LocalToWorld = m_Entity->GetParent()->m_LocalToWorld * localTransform;
        
        m_WorldToLocal = m_LocalToWorld.Inversed();
        
        m_IsDirty = false;
    }
}
