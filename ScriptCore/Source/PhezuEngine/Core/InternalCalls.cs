using System;
using System.Runtime.CompilerServices;

namespace PhezuEngine {

    public static class InternalCalls {

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string Entity_GetTag(ulong entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_HasComponent(ulong entityID, IntPtr componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern IntPtr Entity_GetComponent(ulong entityID, IntPtr componentType);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern IntPtr Entity_RemoveComponent(ulong entityID, IntPtr componentType);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern IntPtr Entity_Instantiate(ulong guid);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_Destroy(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_GetPosition(ulong entityID, out Vector2 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_SetPosition(ulong entityID, ref Vector2 position);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Physics_GetVelocity(ulong entityID, out Vector2 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Physics_SetVelocity(ulong entityID, ref Vector2 velocity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Renderer_GetColor(ulong entityID, string propertyName, out Color value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Renderer_SetColor(ulong entityID, string propertyName, ref Color value);
    }
}
