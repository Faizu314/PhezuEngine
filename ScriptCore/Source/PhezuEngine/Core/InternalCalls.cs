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
        internal static extern ulong Renderer_GetMaterial(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Renderer_SetMaterial(ulong entityID, ulong materialID);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Material_Create(ulong sourceMaterialID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Material_Get(ulong materialGuid);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Material_Destroy(ulong materialID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Material_GetColor(ulong materialID, string propertyName, out Color value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Material_SetColor(ulong materialID, string propertyName, ref Color value);
    }
}
