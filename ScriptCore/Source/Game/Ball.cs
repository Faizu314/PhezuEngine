using System;
using PhezuEngine;

namespace Game {

    public class Ball : BehaviourComponent
    {

        public Vector2 Position => m_Transform.Position;
        
        private Transform m_Transform;
        private Physics m_Physics;
        private float m_Speed;

        private const float THRESHOLD = 0.5f;

        private void OnCreate() {
            m_Transform = Entity.GetComponent<Transform>();
            m_Physics = Entity.GetComponent<Physics>();
        }

        public void Reset(Vector2 velocity)
        {
            m_Transform.Position = new(0f, 0f);
            m_Physics.Velocity = velocity;
            m_Speed = m_Physics.Velocity.Magnitude();
        }
        
        private void OnCollisionEnter(IntPtr otherEntity) {
            Entity other = Entity.GetEntity(otherEntity);
            Brick brick = other.GetComponent<Brick>();

            if (brick == null)
                return;

            brick.OnHit();
        }

        public void AddVelocity(Vector2 velocity)
        {
            Vector2 vel = m_Physics.Velocity + velocity;
            vel.Normalize();
            if (vel.X > THRESHOLD)
                vel.X = THRESHOLD;
            if (vel.X < -THRESHOLD)
                vel.X = -THRESHOLD;
            vel.Normalize();
            vel *= m_Speed;
            
            m_Physics.Velocity = vel;
        }
    }
}
