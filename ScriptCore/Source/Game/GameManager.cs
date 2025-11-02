using PhezuEngine;
using System.Collections.Generic;

namespace Game
{
    public class GameManager : BehaviourComponent, IBrickListener
    {
        private PrefabRef m_BrickPrefabRef;
        private PrefabRef m_BallPrefabRef;
        private PrefabRef m_PlayerPrefabRef;
        
        private List<Brick> m_Bricks;
        private Ball m_Ball;
        private Player m_Player;
        private int m_CurrentLevel = 0;
        private bool m_IsWaitingForNextInput;
        private bool m_IsWaitingToStart;

        private void OnCreate()
        {
            m_Bricks = new();
            m_Ball = Entity.Instantiate(m_BallPrefabRef).GetComponent<Ball>();
            m_Player = Entity.Instantiate(m_PlayerPrefabRef).GetComponent<Player>();
            
            LoadLevel(m_CurrentLevel);
        }

        private void OnUpdate(float deltaTime) {
            if (m_IsWaitingForNextInput) {
                if (!Input.Any) {
                    m_IsWaitingForNextInput = false;
                    m_IsWaitingToStart = true;
                }
                return;
            }

            if (!m_IsWaitingToStart) {
                if (Input.Space) {
                    UnloadLevel();
                    m_CurrentLevel++;
                    LoadLevel(m_CurrentLevel % Level.Levels.Length);
                }
                return;
            }

            if (Input.Any) {
                StartLevel();
                m_IsWaitingToStart = false;
            }
        }

        private void StartLevel() {
            m_Ball.Start(new Vector2(40f, 42f));
            m_Player.Start();
        }
        
        private void LoadLevel(int levelIndex)
        {
            int[,] grid = Level.Levels[levelIndex].GridData;
            Vector2 topLeft = GetTopLeftPosition(grid.GetLength(1));
            Vector2 cellSize = Level.CellSize;

            for (int x = 0; x < grid.GetLength(1); x++)
            {
                for (int y = 0; y < grid.GetLength(0); y++) {
                    if (grid[y, x] == 0)
                        continue;
                    
                    Vector2 worldPos = GridToWorldPosition(x, y, topLeft, cellSize);
                    BrickData brickData = GetBrickData(grid[y, x], worldPos);
                    
                    CreateBrick(brickData);
                }
            }

            m_Ball.Stop();
            m_Player.Stop();
            m_IsWaitingForNextInput = true;
        }

        private Vector2 GetTopLeftPosition(int columnCount) {
            return new Vector2(
                -(columnCount - 1) * Level.CellSize.X / 2f,
                Level.MaxY
            );
        }

        private BrickData GetBrickData(int encoding, Vector2 worldPos) {
            BrickData brickData = default;

            brickData.Position = worldPos;
            
            if (encoding < 10) {
                brickData.Colors = Level.Bricks[0];
                brickData.Health = -1;
            }
            else {
                int type = encoding / 10;
                int health = encoding % 10;
                
                brickData.Colors = Level.Bricks[type];
                brickData.Health = health;
            }
            
            return brickData;
        }

        private void CreateBrick(BrickData brickData) {
            Entity brickEntity = Entity.Instantiate(m_BrickPrefabRef);
            Brick brick = brickEntity.GetComponent<Brick>();
            brick.Initialize(this, brickData);
            m_Bricks.Add(brick);
        }

        void IBrickListener.OnBrickDestroyed(Brick brick)
        {
            m_Bricks.Remove(brick);

            if (CheckLevelComplete())
                LoadNextLevel();
        }

        private bool CheckLevelComplete()
        {
            if (m_Bricks.Count > 0) {
                foreach (var brick in m_Bricks) {
                    if (!brick.IsUnbreakable)
                        return false;
                }
            }

            return true;
        }

        private void LoadNextLevel() {
            UnloadLevel();

            m_CurrentLevel++;

            LoadLevel(m_CurrentLevel % Level.Levels.Length);
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
}