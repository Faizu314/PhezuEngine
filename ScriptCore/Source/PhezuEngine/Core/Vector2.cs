using System;
using System.Runtime.InteropServices;

namespace PhezuEngine {

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2 {
        public float X;
        public float Y;

        public Vector2(float x, float y) {
            X = x;
            Y = y;
        }
        
        public static Vector2 Zero => new Vector2(0, 0);
        public static Vector2 One => new Vector2(1, 1);
        public static Vector2 Right => new Vector2(1, 0);
        public static Vector2 Up => new Vector2(0, 1);
        public static Vector2 Down => new Vector2(0, -1);
        public static Vector2 Left => new Vector2(-1, 0);
        
        public static float Dot(Vector2 lhs, Vector2 rhs) => lhs.X * rhs.X + lhs.Y * rhs.Y;

        public float Magnitude()
        {
            return MathF.Sqrt(X * X + Y * Y);
        }

        public void Normalize()
        {
            float magnitude = Magnitude();

            if (magnitude < float.Epsilon)
                return;
            
            X /= magnitude;
            Y /= magnitude;
        }

        public static Vector2 operator+(Vector2 a, Vector2 b) {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        public static Vector2 operator-(Vector2 a, Vector2 b) {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }

        public static Vector2 operator*(Vector2 a, float b)
        {
            return new Vector2(a.X * b, a.Y * b);
        }
    }
}