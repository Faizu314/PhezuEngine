#pragma warning disable CS0649

namespace PhezuEngine {

    public static class Input {
        internal static bool _W;
        internal static bool _A;
        internal static bool _S;
        internal static bool _D;
        internal static bool _Space;
        
        public static bool Any => _W || _A || _S || _D || _Space;
        public static bool W => _W;
        public static bool A => _A;
        public static bool S => _S;
        public static bool D => _D;
        public static bool Space => _Space;
    }
}
