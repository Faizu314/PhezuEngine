using System;
using PhezuEngine;

namespace Game {

    public class Player : BehaviourComponent {

        private Transform m_Transform;
        private PlayerInput m_Input;
        private float m_Speed = 50f;
        private float m_InputFactor = 20f;

        private void OnCreate() {
            m_Transform = Entity.GetComponent<Transform>();
            m_Input = Entity.GetComponent<PlayerInput>();
        }

        private void OnUpdate(float deltaTime) {
            m_Transform.Position += new Vector2(m_Input.MoveDir * m_Speed * deltaTime, 0f);
        }
        
        private void OnCollisionEnter(IntPtr otherEntity) {
            Entity other = Entity.GetEntity(otherEntity);
            
            if (other.Tag != "Ball")
                return;

            Ball ball = other.GetComponent<Ball>();

            if (ball == null)
                return;

            Vector2 inputComponent = new Vector2(m_Input.MoveDir * m_InputFactor, 0f);
            
            ball.AddVelocity(inputComponent);
        }
    }
}
