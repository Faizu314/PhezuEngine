using PhezuEngine;

namespace Game {

    public class PlayerInput : BehaviourComponent {

        public float MoveDir;

        private void OnUpdate(float deltaTime) {
            MoveDir = 0f;

            if (Input.A)
                MoveDir--;
            if (Input.D)
                MoveDir++;
        }
    }
}
