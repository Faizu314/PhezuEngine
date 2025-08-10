using PhezuEngine;

namespace Game {

    public class PlayerInput : BehaviourComponent {

        public float MoveDir;

        public override void OnCreated() {

        }

        public override void OnDestroyed() {

        }

        public override void OnUpdate(float deltaTime) {
            MoveDir = 0f;

            if (Input.A)
                MoveDir--;
            if (Input.D)
                MoveDir++;
        }
    }
}
