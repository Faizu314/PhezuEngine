using System;
using PhezuEngine;

namespace Game {

    public class Player : BehaviourComponent {

        public override void OnCreated() {
            Console.WriteLine("Created Player");
        }

        public override void OnDestroyed() {
            Console.WriteLine("Destroyed Player");
        }

        public override void OnUpdate(float deltaTime) {
            Console.WriteLine("Player.OnUpdate: " + deltaTime);
        }
    }
}