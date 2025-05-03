using System;
using PhezuEngine;

namespace Game {

    public class Player : BehaviourComponent {

        private Transform m_Transform;

        public override void OnCreated() {
            Console.WriteLine("Player.OnCreated | Entity: " + Entity);
            m_Transform = Entity.GetComponent<Transform>();
        }

        public override void OnDestroyed() {
            Console.WriteLine("Player.OnDestroyed");
        }

        public override void OnUpdate(float deltaTime) {
            m_Transform.Position = m_Transform.Position + new Vector2(10f * deltaTime, 0f);
        }
    }
}