using System;
using PhezuEngine;

namespace Game {

    public class Ball : BehaviourComponent {

        private Physics m_Physics;

        public override void OnCreated() {
            m_Physics = Entity.GetComponent<Physics>();
            m_Physics.Velocity = new Vector2(200f, -220f);
        }

        public override void OnDestroyed() {
            Console.WriteLine("Player.OnDestroyed");
        }

        public override void OnUpdate(float deltaTime) {
            
        }
        
        private void OnCollisionEnter(IntPtr otherEntity) {
            Entity other = Entity.GetEntity(otherEntity);
            
            if (other.Tag != "Paddle")
                return;
            
            Console.WriteLine("CollisionEnter: " + other.Tag);
        }
        
        private void OnCollisionStay(IntPtr otherEntity) {
            Entity other = Entity.GetEntity(otherEntity);
            
            if (other.Tag != "Paddle")
                return;
            
            Console.WriteLine("CollisionStay: " + other.Tag);
        }
        
        private void OnCollisionExit(IntPtr otherEntity) {
            Entity other = Entity.GetEntity(otherEntity);
            
            if (other.Tag != "Paddle")
                return;
            
            Console.WriteLine("CollisionExit: " + other.Tag);
        }
    }
}
