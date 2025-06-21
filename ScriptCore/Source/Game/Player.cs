using System;
using PhezuEngine;

namespace Game {

    public class Player : BehaviourComponent {

        private Transform m_Transform;
        private PlayerInput m_Input;

        public override void OnCreated() {
            m_Transform = Entity.GetComponent<Transform>();
            m_Input = Entity.GetComponent<PlayerInput>();
            Console.WriteLine("Has PlayerInput: " + Entity.HasComponent<PlayerInput>());
        }

        public override void OnDestroyed() {
            Console.WriteLine("Player.OnDestroyed");
        }

        public override void OnUpdate(float deltaTime) {
            m_Transform.Position = m_Transform.Position + new Vector2(10f * deltaTime, 0f);
        }
    }
}