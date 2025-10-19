using PhezuEngine;
using System.Collections.Generic;

namespace Game
{
    public class GameManager : BehaviourComponent
    {
        private LevelData[] Levels;
        private PrefabRef m_BrickPrefabRef;
        private List<Entity> m_Bricks;

        private void OnCreate()
        {
           Levels = new LevelData[1] {
                new LevelData()
                {
                    GridData = new int[,]
                    {
                        { 0, 0, 0, 1 },
                        { 1, 1, 0, 0 }
                    },
                    CellSize = new Vector2(3.1f, 1.1f),
                    TopLeft = new Vector2(-8f, 9f)
                }
            };
            
            m_Bricks = new();

            LoadLevel(0);
        }

        private void LoadLevel(int levelIndex)
        {
            Vector2 topLeft = Levels[levelIndex].TopLeft;
            Vector2 cellSize = Levels[levelIndex].CellSize;
            int[,] grid = Levels[levelIndex].GridData;

            for (int x = 0; x < grid.GetLength(1); x++)
            {
                for (int y = 0; y < grid.GetLength(0); y++)
                {
                    Vector2 worldPos = GridToWorldPosition(x, y, topLeft, cellSize);
                    
                    switch (grid[y, x])
                    {
                        case 0:
                            continue;
                        case 1:
                            Entity brick = Entity.Instantiate(m_BrickPrefabRef);
                            brick.GetComponent<Transform>().Position = worldPos;
                            m_Bricks.Add(brick);
                            break;
                    }
                }
            }
        }

        private void UnloadLevel()
        {
            foreach (var brick in m_Bricks)
                brick.Destroy();
            m_Bricks.Clear();
        }

        private Vector2 GridToWorldPosition(int gridX, int gridY, Vector2 topLeft, Vector2 cellSize)
        {
            float x = topLeft.X + gridX * cellSize.X;
            float y = topLeft.Y - gridY * cellSize.Y;
            
            return new Vector2(x, y);
        }
    }
    
    public struct LevelData
    {
        public int[,] GridData;
        public Vector2 CellSize;
        public Vector2 TopLeft;
    }
}