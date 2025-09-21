using System;
using System.Runtime.InteropServices;

namespace PhezuEngine {

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2 {
        public float X;
        public float Y;

        public Vector2(float x, float y) {
            this.X = x;
            this.Y = y;
        }

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

        public static Vector2 operator *(Vector2 a, float b)
        {
            return new Vector2(a.X * b, a.Y * b);
        }
    }
}