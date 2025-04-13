using System;
using PhezuEngine;

namespace Game {

    public class Player : BehaviourComponent {
        public override void OnCreated() {

        }

        public override void OnDestroyed() {

        }

        public override void OnUpdate(float deltaTime) {
            Console.WriteLine("OnUpdate method from C#: " + deltaTime);
        }
    }
}