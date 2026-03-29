// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Mat3x3.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class TransformData : public DataComponent {
    public:
        TransformData(Entity* entity, Vector2 localPosition = Vector2::Zero, float localRotation = 0.0f, Vector2 localScale = Vector2::One);
    public:
        Vector2 GetLocalPosition() const { return m_LocalPosition; }
        Vector2 GetWorldPosition() const;
        Vector2 GetLocalScale() const { return m_LocalScale; }
        void SetLocalPosition(Vector2 position);
        void SetWorldPosition(Vector2 position);
        void SetLocalScale(Vector2 scale);
        void SetLocalRotation(float rotation);
    public:
        Vector2 LocalToWorldPoint(Vector2 localPoint) const;
        Vector2 WorldToLocalPoint(Vector2 worldPoint) const;
        const Mat3x3& GetLocalToWorld() const { return m_LocalToWorld; }
    public:
        void RecalculateLocalToWorld();
        bool GetIsDirty() { return m_IsDirty; }
    private:
        Vector2 m_LocalPosition;
        Vector2 m_LocalScale;
        float m_LocalRotation;
        bool m_IsDirty;
    private:
        Mat3x3 m_LocalToWorld;
        Mat3x3 m_WorldToLocal;
    };
}
