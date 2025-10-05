using System;
using PhezuEngine;
using System.Collections.Generic;

namespace Game
{
    public class GameManager : BehaviourComponent
    {
        private PrefabRef m_BrickPrefabRef;

        private List<Entity> m_Bricks;

        private void OnCreate()
        {
            m_Bricks = new();
            
            Entity brick = Entity.Instantiate(m_BrickPrefabRef);
            Console.WriteLine("HasBall: " + brick.HasComponent<Ball>());
            brick.RemoveComponent<Ball>();

            
            var transform = brick.GetComponent<Transform>();
            transform.Position = new Vector2(0f, 100f);
        }

        private void LoadLevel(int[,] levelData)
        {
            Entity brick = Entity.Instantiate(m_BrickPrefabRef);
            m_Bricks.Add(brick);
        }
    }
}