using System;
using PhezuEngine;

namespace Game {

    public class PlayerInput : BehaviourComponent {

        public Vector2 MoveDir;

        public override void OnCreated() {

        }

        public override void OnDestroyed() {

        }

        public override void OnUpdate(float deltaTime) {
            MoveDir.X = 0f;
            MoveDir.Y = 0f;

            if (Input.W)
                MoveDir.Y++;
            if (Input.A)
                MoveDir.X--;
            if (Input.S)
                MoveDir.Y--;
            if (Input.D)
                MoveDir.X++;
        }
    }
}
