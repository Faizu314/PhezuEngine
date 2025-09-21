using System;
using PhezuEngine;

namespace Game {

    public class Ball : BehaviourComponent
    {

        public Vector2 Position => m_Transform.Position;
        
        private Transform m_Transform;
        private Physics m_Physics;
        private float m_Speed;

        public override void OnCreated() {
            m_Transform = Entity.GetComponent<Transform>();
            m_Physics = Entity.GetComponent<Physics>();
            m_Physics.Velocity = new Vector2(400f, -420f);
            m_Speed = m_Physics.Velocity.Magnitude();
        }

        public override void OnDestroyed() {
            Console.WriteLine("Player.OnDestroyed");
        }

        public override void OnUpdate(float deltaTime)
        {
            
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
