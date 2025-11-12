using PhezuEngine;

namespace Game {
    
    public static class Level {

        public static Vector2 CellSize = new Vector2(2.1f, 0.8f);
        public static float MaxY = 9f;
        
        public static LevelData[] Levels = {
            new() {
                LevelNumber = 0,
                GridData = new[,] {
                    { 0, 0, 0, 0, 0, 0, 0, 0 },
                    { 0, 11, 11, 11, 11, 11, 11, 0 },
                    { 11, 11, 11, 11, 11, 11, 11, 11 }
                }
            },
            new() {
                LevelNumber = 1,
                GridData = new[,] {
                    { 0, 0, 11, 12, 11, 12, 11, 0, 0 },
                    { 0, 11, 12, 11, 12, 11, 12, 11, 0 },
                    { 11, 11, 12, 11, 12, 11, 11, 11, 11 }
                }  
            },
            new() {
                LevelNumber = 2,
                GridData = new[,] {
                    { 0, 11, 12, 12, 12, 12, 11, 0 },
                    { 11, 0, 0, 0, 0, 0, 0, 11 },
                    { 11, 0, 11, 11, 11, 11, 0, 11 },
                    { 0, 11, 12, 12, 12, 12, 11, 0 }
                }  
            },
            new() {
                LevelNumber = 3,
                GridData = new[,] {
                    {  0,  0,  0,  0,  0,  0 },
                    {  0,  0, 11, 11,  0,  0 },
                    {  0, 11, 11, 11, 11,  0 },
                    {  1, 11, 11, 11, 11,  1 },
                    {  0,  1, 11, 11,  1,  0 },
                    {  0,  0, 11, 11,  0,  0 }
                }
            },
            new() {
                LevelNumber = 4,
                GridData = new[,] {
                    {  0,  0,  0,  0,  0,  0,  0,  0,  0 },
                    {  0,  0,  0, 12,  0, 12,  0,  0,  0 },
                    {  0,  0, 12,  0, 13,  0, 12,  0,  0 },
                    {  0, 12,  0, 14,  0, 14,  0, 12,  0 },
                    { 12,  0, 14,  0, 11,  0, 14,  0, 13 },
                    {  0, 12,  0, 11, 11, 11,  0, 13,  0 },
                    { 12,  0, 14,  0, 11,  0, 14,  0, 13 },
                    {  0, 12,  0, 14,  0, 14,  0, 12,  0 },
                    {  0,  0, 12,  0, 13,  0, 12,  0,  0 },
                    {  0,  0,  0, 12,  0, 12,  0,  0,  0 }
                }
            },
            new() { 
                LevelNumber = 5,
                GridData = new[,] {
                    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
                    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
                    {  0, 21, 22,  0,  0,  0,  0, 22, 21,  0 },
                    { 22,  0,  0,  0, 23, 23,  0,  0,  0, 22 },
                    {  0, 21, 24,  0, 23,  0, 23,  0, 24, 21 },
                    {  0,  0, 22, 23,  0,  0, 23, 22,  0,  0 },
                    {  0,  0,  0, 24, 25, 25, 24,  0,  0,  0 }
                }
            },
            new() {
                LevelNumber = 6,
                GridData = new[,] {
                    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
                    {  0,  0, 11, 11, 11, 11, 11, 11,  0,  0 },
                    {  0, 11, 11, 11, 11, 11, 11, 11, 11,  0 },
                    {  11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
                    {  0, 11, 11, 11, 11, 11, 11, 11, 11,  0 },
                    {  0,  0, 11, 11, 11, 11, 11, 11,  0,  0 },
                    {  0,  0,  0, 11, 11, 11, 11,  0,  0,  0 }
                }
            },
            new() {
                LevelNumber = 7,
                GridData = new[,] {
                    { 31, 31, 32, 33, 34, 35, 34, 33, 32, 31, 31, 31 },
                    { 31, 21, 21, 22, 23, 24, 23, 22, 21, 21, 21, 31 },
                    { 31, 21, 11, 12, 13, 21, 21, 13, 12, 11, 21, 31 },
                    { 31, 21, 11,  1,  1, 21, 21,  1,  1, 11, 21, 31 },
                    { 31, 21, 11,  1,  1, 21, 21,  1,  1, 11, 21, 31 },
                    { 31, 21, 11, 12, 13, 21, 21, 13, 12, 11, 21, 31 },
                    { 31, 21, 21, 22, 23, 24, 23, 22, 21, 21, 21, 31 },
                    { 31, 31, 32, 33, 34, 35, 34, 33, 32, 31, 31, 31 }
                }
            },
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
                new Color(20, 100, 20),  // Health 1 — light green
                new Color(40, 120, 40),    // Health 4
                new Color(60, 150, 60),    // Health 3
                new Color(80, 180, 80),    // Health 2 — slightly darker
                new Color(20, 200, 20)     // Health 5 — bright green, strong contrast
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
            public int LevelNumber;
            public int[,] GridData;
        }
    }
}