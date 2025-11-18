#include "Physics.hpp"
#include "Core/Engine.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/PhysicsData.hpp"
#include "Scene/Components/ShapeData.hpp"

#include <algorithm>

namespace Phezu {
    
    const float Physics::EPSILON = 0.000000;
    
    Physics::Physics(Engine* engine) : m_Engine(engine), m_DeltaTime(0) {}
    
    void Physics::CleanCollidingEntities() {
        //Physics should receive a callback on physics component being destroyed/removed
        
        //for (auto it = m_CollidingEntities.begin(); it != m_CollidingEntities.end();) {
        //    auto e1 = (*it).first;
        //    auto e2 = (*it).second;
        //    
        //    if (e1.expired() || e2.expired()) {
        //        if (e1.expired() && !e2.expired())
        //            e2.lock()->OnCollisionExit(Collision(e1));
        //        else if (e2.expired() && !e1.expired())
        //            e1.lock()->OnCollisionExit(Collision(e2));
        //        it = m_CollidingEntities.erase(it);
        //    }
        //    else
        //        it++;
        //}
    }
    
    void Physics::PhysicsUpdate(const std::vector<Entity*>& staticEntities, const std::vector<Entity*>& dynamicEntities, size_t staticCount, size_t dynamicCount, float deltaTime) {
        m_DeltaTime = deltaTime;
        
        CleanCollidingEntities();
        
        for (size_t i = 0; i < dynamicCount; i++) {
            auto dynamicEntity = dynamicEntities[i];
            TransformData* trans = dynamic_cast<TransformData*>(dynamicEntity->GetDataComponent(ComponentType::Transform));
            PhysicsData* phys = dynamic_cast<PhysicsData*>(dynamicEntity->GetDataComponent(ComponentType::Physics));
            trans->SetWorldPosition(trans->GetWorldPosition() + (phys->Velocity * deltaTime));
            dynamicEntity->RecalculateSubtreeTransforms();
        }
        
        for (size_t i = 0; i < dynamicCount; i++) {
            auto dynamicEntity = dynamicEntities[i];
                
            ResolveDynamicToStaticCollisions(dynamicEntity, staticEntities, staticCount);
        }
        
        ResolveDynamicToDynamicCollisions(dynamicEntities, dynamicCount);
    }
    
    void Physics::ResolveDynamicToDynamicCollisions(const std::vector<Entity*> &dynamicEntities, size_t entitiesCount) {
        CollisionData collisionData;
        
        for (size_t i = 0; i < entitiesCount; i++) {
            for (size_t j = i + 1; j < entitiesCount; j++) {
                auto d1 = dynamicEntities[i];
                auto d2 = dynamicEntities[j];
                
                if (!IsColliding(d1, d2, collisionData)) {
                    OnNotColliding(d1, d2);
                    continue;
                }
                
                ResolveDynamicToDynamicCollision(d1, d2, collisionData);
                OnColliding(d1, d2);
            }
        }
    }
    
    void Physics::ResolveDynamicToDynamicCollision(Entity* d1, Entity* d2, CollisionData& cd) {
        glm::mat2 reflectionMat(1);
        
        float xTranslate = 0, yTranslate = 0;
        float downPenY = glm::max(0.0f, cd.A.MaxY - cd.B.MinY);
        float upPenY = glm::max(0.0f, cd.B.MaxY - cd.A.MinY);
        float leftPenX = glm::max(0.0f, cd.A.MaxX - cd.B.MinX);
        float rightPenX = glm::max(0.0f, cd.B.MaxX - cd.A.MinX);
        
        if (rightPenX <= glm::min(downPenY, upPenY) && rightPenX <= leftPenX) {
            xTranslate = cd.B.MaxX - cd.A.MinX + EPSILON;
            reflectionMat[0].x = -1;
        }
        else if (leftPenX <= glm::min(downPenY, upPenY) && rightPenX >= leftPenX) {
            xTranslate = cd.B.MinX - cd.A.MaxX - EPSILON;
            reflectionMat[0].x = -1;
        }
        if (upPenY <= glm::min(leftPenX, rightPenX) && upPenY <= downPenY) {
            yTranslate = cd.B.MaxY - cd.A.MinY + EPSILON;
            reflectionMat[1].y = -1;
        }
        else if (downPenY <= glm::min(leftPenX, rightPenX) && upPenY >= downPenY) {
            yTranslate = cd.B.MinY - cd.A.MaxY - EPSILON;
            reflectionMat[1].y = -1;
        }
        
        auto* phys1 = dynamic_cast<PhysicsData*>(d1->GetDataComponent(ComponentType::Physics));
        auto* phys2 = dynamic_cast<PhysicsData*>(d2->GetDataComponent(ComponentType::Physics));
        
        glm::vec reflectedVel1 = reflectionMat * phys1->Velocity;
        glm::vec reflectedVel2 = reflectionMat * phys2->Velocity;
        phys1->Velocity = Vector2(reflectedVel1.x, reflectedVel1.y);
        phys2->Velocity = Vector2(reflectedVel2.x, reflectedVel2.y);
        
        if ((phys1->Velocity.SqrLength() > 0 && phys2->Velocity.SqrLength() > 0) || (phys1->Velocity.SqrLength() == 0 && phys2->Velocity.SqrLength() == 0)) {
            auto trans1 = dynamic_cast<TransformData*>(d1->GetDataComponent(ComponentType::Transform));
            trans1->SetLocalPosition(trans1->GetLocalPosition() + Vector2(xTranslate / 2, yTranslate / 2));
            d1->RecalculateSubtreeTransforms();
            
            auto trans2 = dynamic_cast<TransformData*>(d2->GetDataComponent(ComponentType::Transform));
            trans2->SetLocalPosition(trans2->GetLocalPosition() + Vector2(-xTranslate / 2, -yTranslate / 2));
            d2->RecalculateSubtreeTransforms();
        }
        else if (phys1->Velocity.SqrLength() > 0) {
            auto trans1 = dynamic_cast<TransformData*>(d1->GetDataComponent(ComponentType::Transform));
            trans1->SetLocalPosition(trans1->GetLocalPosition() + Vector2(xTranslate, yTranslate));
            d1->RecalculateSubtreeTransforms();
        }
        else {
            auto trans2 = dynamic_cast<TransformData*>(d2->GetDataComponent(ComponentType::Transform));
            trans2->SetLocalPosition(trans2->GetLocalPosition() + Vector2(-xTranslate, -yTranslate));
            d2->RecalculateSubtreeTransforms();
        }
    }
    
    void Physics::ResolveDynamicToStaticCollisions(Entity* dynamicEntity, const std::vector<Entity*>& staticEntities, size_t staticCount) {
        CollisionData collisionData;
        
        for (size_t i = 0; i < staticCount; i++) {
            auto staticEntity = staticEntities[i];

            if (!IsColliding(dynamicEntity, staticEntity, collisionData)) {
                OnNotColliding(dynamicEntity, staticEntity);
                continue;
            }
            
            ResolveDynamicToStaticCollision(dynamicEntity, staticEntity, collisionData);
            OnColliding(dynamicEntity, staticEntity);
        }
    }
    
    Physics::EntityRect Physics::GetWorldRectFromTransformAndShapeData(TransformData* transData, ShapeData* shapeData) {
        Vector2 ul = transData->LocalToWorldPoint(shapeData->GetVertexPosition(ShapeData::VertexType::UpLeft));
        Vector2 dr = transData->LocalToWorldPoint(shapeData->GetVertexPosition(ShapeData::VertexType::DownRight));
        
        EntityRect rect;
        
        rect.MaxX = glm::max(ul.X(), dr.X());
        rect.MinX = glm::min(ul.X(), dr.X());
        rect.MaxY = glm::max(ul.Y(), dr.Y());
        rect.MinY = glm::min(ul.Y(), dr.Y());
        
        return rect;
    }
    
    bool Physics::IsColliding(Entity* entityA, Entity* entityB, CollisionData& cd) {
        auto transA = dynamic_cast<TransformData*>(entityA->GetDataComponent(ComponentType::Transform));
        auto shapeA = dynamic_cast<ShapeData*>(entityA->GetDataComponent(ComponentType::Shape));
        auto transB = dynamic_cast<TransformData*>(entityB->GetDataComponent(ComponentType::Transform));
        auto shapeB = dynamic_cast<ShapeData*>(entityB->GetDataComponent(ComponentType::Shape));

        cd.A = GetWorldRectFromTransformAndShapeData(transA, shapeA);
        cd.B = GetWorldRectFromTransformAndShapeData(transB, shapeB);
        
        bool overlapX = cd.A.MinX < cd.B.MaxX && cd.A.MaxX > cd.B.MinX;
        bool overlapY = cd.A.MinY < cd.B.MaxY && cd.A.MaxY > cd.B.MinY;
        
        return overlapX && overlapY;
    }
    
    void Physics::ResolveDynamicToStaticCollision(Entity* dynamicEntity, Entity* staticEntity, CollisionData& cd) {
        glm::mat2 reflectionMat(1);
        
        float xTranslate = 0, yTranslate = 0;
        float downPenY = glm::max(0.0f, cd.A.MaxY - cd.B.MinY);
        float upPenY = glm::max(0.0f, cd.B.MaxY - cd.A.MinY);
        float leftPenX = glm::max(0.0f, cd.A.MaxX - cd.B.MinX);
        float rightPenX = glm::max(0.0f, cd.B.MaxX - cd.A.MinX);
        
        if (rightPenX <= glm::min(downPenY, upPenY) && rightPenX <= leftPenX) {
            xTranslate = cd.B.MaxX - cd.A.MinX + EPSILON;
            reflectionMat[0].x = -1;
        }
        else if (leftPenX <= glm::min(downPenY, upPenY) && rightPenX >= leftPenX) {
            xTranslate = cd.B.MinX - cd.A.MaxX - EPSILON;
            reflectionMat[0].x = -1;
        }
        if (upPenY <= glm::min(leftPenX, rightPenX) && upPenY <= downPenY) {
            yTranslate = cd.B.MaxY - cd.A.MinY + EPSILON;
            reflectionMat[1].y = -1;
        }
        else if (downPenY <= glm::min(leftPenX, rightPenX) && upPenY >= downPenY) {
            yTranslate = cd.B.MinY - cd.A.MaxY - EPSILON;
            reflectionMat[1].y = -1;
        }
        
        auto trans = dynamic_cast<TransformData*>(dynamicEntity->GetDataComponent(ComponentType::Transform));
        auto phys = dynamic_cast<PhysicsData*>(dynamicEntity->GetDataComponent(ComponentType::Physics));
        
        glm::vec reflectedVel = reflectionMat * phys->Velocity;
        phys->Velocity = Vector2(reflectedVel.x, reflectedVel.y);
        
        trans->SetLocalPosition(trans->GetLocalPosition() + Vector2(xTranslate, yTranslate));
        dynamicEntity->RecalculateSubtreeTransforms();
    }
    
    void Physics::OnColliding(Entity* a, Entity* b) {
        auto ap = dynamic_cast<PhysicsData*>(a->GetDataComponent(ComponentType::Physics));
        auto bp = dynamic_cast<PhysicsData*>(b->GetDataComponent(ComponentType::Physics));
        
        Collision cA(bp);
        Collision cB(ap);
        
        if (WereColliding(ap, bp)) {
            m_Engine->GetScriptEngine().FirePhysicsCollisionEvent(a->GetEntityID(), b->GetEntityID(), PhysicsEventType::CollisionStay);
        }
        else {
            m_Engine->GetScriptEngine().FirePhysicsCollisionEvent(a->GetEntityID(), b->GetEntityID(), PhysicsEventType::CollisionEnter);
            m_CollidingEntities.emplace_back(ap, bp);
        }
    }
    
    void Physics::OnNotColliding(Entity* a, Entity* b) {
        auto ap = dynamic_cast<PhysicsData*>(a->GetDataComponent(ComponentType::Physics));
        auto bp = dynamic_cast<PhysicsData*>(b->GetDataComponent(ComponentType::Physics));
        
        Collision cA(bp);
        Collision cB(ap);
        
        if (WereColliding(ap, bp)) {
            m_Engine->GetScriptEngine().FirePhysicsCollisionEvent(a->GetEntityID(), b->GetEntityID(), PhysicsEventType::CollisionExit);
            RemoveCollisionPair(ap, bp);
        }
    }
    
    bool Physics::WereColliding(PhysicsData* a, PhysicsData* b) {
        for (auto& pair : m_CollidingEntities) {
            auto sharedA = pair.first;
            auto sharedB = pair.second;
            auto sharedC = a;
            auto sharedD = b;
            
            if ((sharedA == sharedC && sharedB == sharedD) || (sharedA == sharedD && sharedB == sharedC))
                return true;
        }
        
        return false;
    }
    
    void Physics::RemoveCollisionPair(PhysicsData* a, PhysicsData* b) {
        for (auto it = m_CollidingEntities.begin(); it != m_CollidingEntities.end(); it++) {
            auto pair = *it;
            auto sharedA = pair.first;
            auto sharedB = pair.second;
            auto sharedC = a;
            auto sharedD = b;
            
            if ((sharedA == sharedC && sharedB == sharedD) || (sharedA == sharedD && sharedB == sharedC)) {
                m_CollidingEntities.erase(it);
                return;
            }
        }
    }
}
