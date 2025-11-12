using System.Runtime.InteropServices;

namespace PhezuEngine
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Color
    {
        public byte R;
        public byte G;
        public byte B;
        public byte A;

        public Color(byte r, byte g, byte b, byte a = 255)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }
        
        public static Color White => new Color(255, 255, 255, 255);
        public static Color Red => new Color(255, 0, 0, 255);
    }
}