using System;
using PhezuEngine;

namespace Game {

    public class Ball : BehaviourComponent
    {

        public Vector2 Position => m_Transform.Position;
        
        private Transform m_Transform;
        private Physics m_Physics;
        private float m_Speed;

        private void OnCreate() {
            m_Transform = Entity.GetComponent<Transform>();
            m_Physics = Entity.GetComponent<Physics>();
            m_Physics.Velocity = new Vector2(400f, -420f);
            m_Speed = m_Physics.Velocity.Magnitude();
        }
        
        private void OnCollisionEnter(IntPtr otherEntity) {
            Entity other = Entity.GetEntity(otherEntity);
            
            if (other.Tag != "Brick")
                return;

            other.Destroy();
        }

        public void AddVelocity(Vector2 velocity)
        {
            Vector2 vel = m_Physics.Velocity + velocity;
            vel.Normalize();
            vel *= m_Speed;
            
            m_Physics.Velocity = vel;
        }
    }
}
