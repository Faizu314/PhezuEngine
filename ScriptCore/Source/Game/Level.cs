using PhezuEngine;

namespace Game {
    
    public static class Level {

        public static Vector2 CellSize = new Vector2(2.1f, 0.8f);
        public static Vector2 TopLeft = new Vector2(-11.5f, 9f);
        
        public static LevelData[] Levels = {
            new() {
                GridData = new[,] {
                    {  1,  2,  3,  4,  5,  6,  7,  8,  9, 11, 12, 13 },
                    { 14, 15, 21, 22, 23, 24, 25, 31, 32, 33, 34, 35 },
                    { 11, 21, 31, 12, 22, 32, 13, 23, 33, 14, 24, 34 },
                    { 15, 25, 35, 11, 21, 31, 12, 22, 32, 13, 23, 33 },
                    { 14, 24, 34, 15, 25, 35, 11, 12, 13, 21, 22, 23 },
                    { 24, 31, 32, 33, 34, 35, 11, 14, 15, 21, 25, 12 },
                    { 13, 23, 33, 14, 24, 34, 15, 25, 35, 11, 22, 31 },
                    { 32, 12, 13, 21, 1, 23, 24, 1, 31, 32, 33, 34 },
                }
            },
            new() {
                GridData = new[,] {
                    { 1, 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1, 1 },
                }
            }
        };

        private const int UNBREAKABLE = 0;
        const int WARM = 10;
        const int COOL = 20;
        const int PURPLE = 30;

        public static Color[][] Bricks = {
            new[] { // Unbreakable
                new Color(50, 50, 50)
            },
            new[] { // Warm
                new Color(200, 0, 0), 
                new Color(150, 150, 0), 
                new Color(0, 0, 200),
                new Color(0, 200, 0),
                new Color(200, 200, 200)
            },
            new[] { // Cool
                new Color(0, 120, 200),    // deep blue (low health)
                new Color(0, 170, 200),    // teal
                new Color(0, 200, 180),    // aqua
                new Color(100, 230, 230),  // light cyan
                new Color(200, 200, 200)   // white-blue tint (max health)
            },
            new[] { // Purple
                new Color(150, 0, 150),    // deep purple (low health)
                new Color(180, 0, 180),    // magenta
                new Color(210, 60, 210),   // bright pink
                new Color(230, 130, 230),  // light lavender
                new Color(200, 200, 200)   // soft white tint (max health)
            },
        };

        public struct LevelData {
            public int[,] GridData;
        }
    }
}