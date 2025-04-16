using System;
using PhezuEngine;

namespace Game {

    public class Player : BehaviourComponent {

        public override void OnCreated() {
            Console.WriteLine("Player.OnCreated");
        }

        public override void OnDestroyed() {
            Console.WriteLine("Player.OnDestroyed");
        }

        public override void OnUpdate(float deltaTime) {
            Console.WriteLine("Player.OnUpdate: " + deltaTime);
        }
    }
}