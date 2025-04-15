using System;
using PhezuEngine;

namespace Game {

    public class Obstacle : BehaviourComponent {

        public override void OnCreated() {
            Console.WriteLine("Created Obstacle");
        }

        public override void OnDestroyed() {
            Console.WriteLine("Destroyed Obstacle");
        }

        public override void OnUpdate(float deltaTime) {
            Console.WriteLine("Obstacle.OnUpdate: " + deltaTime);
        }
    }
}