#pragma once

#include <memory>
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
        void PhysicsUpdate(const std::vector<std::weak_ptr<Entity>>& staticEntities, const std::vector<std::weak_ptr<Entity>>& dynamicEntities, size_t staticCount, size_t dynamicCount, float deltaTime);
    private:
        void ResolveDynamicToStaticCollisions(std::shared_ptr<Entity> dynamicEntity, const std::vector<std::weak_ptr<Entity>>& staticEntities, size_t staticCount);
        void ResolveDynamicToStaticCollision(std::shared_ptr<Entity> dynamicEntity, std::shared_ptr<Entity> staticEntity, CollisionData& collisionData);
        void ResolveDynamicToDynamicCollisions(const std::vector<std::weak_ptr<Entity>>& dynamicEntities, size_t entitiesCount);
        void ResolveDynamicToDynamicCollision(std::shared_ptr<Entity> d1, std::shared_ptr<Entity> d2, CollisionData& collisionData);
        bool IsColliding(std::shared_ptr<Entity> entityA, std::shared_ptr<Entity> entityB, CollisionData& collisionData);
        EntityRect GetWorldRectFromTransformAndShapeData(TransformData* transData, ShapeData* shapeData);
    private:
        void CleanCollidingEntities();
        bool WereColliding(PhysicsData* a, PhysicsData* b);
        void OnColliding(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
        void OnNotColliding(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
        void RemoveCollisionPair(PhysicsData* a, PhysicsData* b);
    private:
        Engine* m_Engine;
        std::vector<std::pair<PhysicsData*, PhysicsData*>> m_CollidingEntities;
    private:
        static const float EPSILON;
        float m_DeltaTime;
    };
}
