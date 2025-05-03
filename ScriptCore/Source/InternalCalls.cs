using System;
using System.Runtime.CompilerServices;

namespace PhezuEngine {

    public static class InternalCalls {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static BehaviourComponent Entity_GetComponent(ulong entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetPosition(ulong entityID, out Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetPosition(ulong entityID, ref Vector2 position);
    }
}