using PhezuEngine;
using System.Collections.Generic;

namespace Game
{
    public class GameManager : BehaviourComponent, IBrickListener
    {
        private LevelData[] Levels = new LevelData[] {
            new()
            {
                GridData = new[,]
                {
                    { 1, 1, 1, 1, 1, 1, 1 },
                },
                CellSize = new Vector2(3.1f, 0.8f),
                TopLeft = new Vector2(-9f, 9f)
            }
        };
        
        private PrefabRef m_BrickPrefabRef;
        private PrefabRef m_BallPrefab;
        private List<Brick> m_Bricks;
        private Ball m_Ball;
        private int m_CurrentLevel;

        private void OnCreate()
        {
            m_Bricks = new();
            m_Ball = Entity.Instantiate(m_BallPrefab).GetComponent<Ball>();
            
            LoadLevel(m_CurrentLevel);
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
                            Entity brickEntity = Entity.Instantiate(m_BrickPrefabRef);
                            Brick brick = brickEntity.GetComponent<Brick>();
                            brick.Initialize(this, worldPos);
                            m_Bricks.Add(brick);
                            break;
                    }
                }
            }
            
            m_Ball.Reset(new Vector2(40f, 42f));
        }

        void IBrickListener.OnBrickDestroyed(Brick brick)
        {
            m_Bricks.Remove(brick);
            
            CheckLevelComplete();
        }

        private void CheckLevelComplete()
        {
            if (m_Bricks.Count > 0)
                return;
            
            UnloadLevel();

            m_CurrentLevel++;

            LoadLevel(m_CurrentLevel % Levels.Length);
        }
        
        private void UnloadLevel()
        {
            foreach (var brick in m_Bricks)
                brick.Entity.Destroy();
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