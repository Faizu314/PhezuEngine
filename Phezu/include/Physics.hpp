#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "maths/Math.hpp"

namespace Phezu {
    
    class Engine;
    class Entity;
    class PhysicsData;
    class TransformData;
    class ShapeData;
    
    class Physics {
    private:
        struct EntityRect {
            float MinX, MaxX, MinY, MaxY;
        };
        struct CollisionData {
            EntityRect A, B;
        };
    public:
        Physics(Engine* engine);
    public:
        void PhysicsUpdate(const std::vector<Entity*>& staticEntities, const std::vector<Entity*>& dynamicEntities, size_t staticCount, size_t dynamicCount, float deltaTime);
    private:
        void ResolveDynamicToStaticCollisions(Entity* dynamicEntity, const std::vector<Entity*>& staticEntities, size_t staticCount);
        void ResolveDynamicToStaticCollision(Entity* dynamicEntity, Entity* staticEntity, CollisionData& collisionData);
        void ResolveDynamicToDynamicCollisions(const std::vector<Entity*>& dynamicEntities, size_t entitiesCount);
        void ResolveDynamicToDynamicCollision(Entity* d1, Entity* d2, CollisionData& collisionData);
        bool IsColliding(Entity* entityA, Entity* entityB, CollisionData& collisionData);
        EntityRect GetWorldRectFromTransformAndShapeData(TransformData* transData, ShapeData* shapeData);
    private:
        void CleanCollidingEntities();
        bool WereColliding(PhysicsData* a, PhysicsData* b);
        void OnColliding(Entity* a, Entity* b);
        void OnNotColliding(Entity* a, Entity* b);
        void RemoveCollisionPair(PhysicsData* a, PhysicsData* b);
    private:
        Engine* m_Engine;
        std::vector<std::pair<PhysicsData*, PhysicsData*>> m_CollidingEntities;
    private:
        static const float EPSILON;
        float m_DeltaTime;
    };
}
