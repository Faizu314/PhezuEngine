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
        }

        private void LoadLevel(int[,] levelData)
        {
            Entity brick = Entity.Instantiate(m_BrickPrefabRef);
            m_Bricks.Add(brick);
        }
    }
}