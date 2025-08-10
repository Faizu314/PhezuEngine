using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace PhezuEngine {

    public static class InternalCalls {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_GetTag(ulong entityID);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityID, IntPtr componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static IntPtr Entity_GetComponent(ulong entityID, IntPtr componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetPosition(ulong entityID, out Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetPosition(ulong entityID, ref Vector2 position);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Physics_GetVelocity(ulong entityID, out Vector2 velocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Physics_SetVelocity(ulong entityID, ref Vector2 velocity);
    }
}
