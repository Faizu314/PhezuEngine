using System;
using PhezuEngine;

namespace Game {

    public class Player : BehaviourComponent {

        private Transform m_Transform;
        private PlayerInput m_Input;
        private float m_Speed = 70f;
        private float m_AngleHitFactor = 20f;
        private float m_PositionY = -8f;
        private bool m_IsStopped;
        
        private const float MAX_DISPLACEMENT = 11.75f;

        private void OnCreate() {
            m_Transform = Entity.GetComponent<Transform>();
            m_PositionY = m_Transform.Position.Y;
            m_Input = Entity.GetComponent<PlayerInput>();
        }

        private void OnUpdate(float deltaTime) {
            if (m_IsStopped)
                return;
            
            Vector2 currPos = m_Transform.Position;
            
            if (currPos.X >= MAX_DISPLACEMENT && m_Input.MoveDir > 0f)
                m_Transform.Position = new Vector2(MAX_DISPLACEMENT, m_PositionY);
            else if (currPos.X <= -MAX_DISPLACEMENT && m_Input.MoveDir < 0f)
                m_Transform.Position = new Vector2(-MAX_DISPLACEMENT, m_PositionY);
            else
                m_Transform.Position += new Vector2(m_Input.MoveDir * m_Speed * deltaTime, 0f);
        }

        public void Start() {
            m_IsStopped = false;
        }

        public void Stop() {
            m_Transform.Position = new(0f, m_PositionY);
            m_IsStopped = true;
        }
        
        private void OnCollisionEnter(IntPtr otherEntity) {
            Entity other = Entity.GetEntity(otherEntity);
            
            if (other.Tag != "Ball")
                return;

            Ball ball = other.GetComponent<Ball>();

            if (ball == null)
                return;
            
            Vector2 ballPos = ball.Position;
            float hitAngleMag = (ballPos - m_Transform.Position).X * m_AngleHitFactor;
            
            ball.AddVelocity(Vector2.Right * hitAngleMag);
        }
    }
}
