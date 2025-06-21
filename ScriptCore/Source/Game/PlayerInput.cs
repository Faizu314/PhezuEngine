using System;
using PhezuEngine;

namespace Game {

    public class PlayerInput : BehaviourComponent {

        public Vector2 MoveDir;
        public float InitedStatically = 0.45f;
        public float InitedOnCreate;

        public override void OnCreated() {
            InitedOnCreate = 0.245f;
            Console.WriteLine("PlayerInput.OnCreated | Entity: " + Entity);
        }

        public override void OnDestroyed() {
            Console.WriteLine("PlayerInput.OnDestroyed");
        }

        public override void OnUpdate(float deltaTime) {

        }
    }
}