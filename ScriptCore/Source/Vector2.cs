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

        public static Vector2 operator+(Vector2 a, Vector2 b) {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }
    }
}