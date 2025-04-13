using System;
using System.Runtime.CompilerServices;

namespace PhezuEngine {

    public static class InternalCalls {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Transform_GetPosition(ulong entityID, out Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Transform_SetPosition(ulong entityID, ref Vector2 position);
    }
}