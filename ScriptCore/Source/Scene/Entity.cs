using System;
using System.Runtime.InteropServices;

namespace PhezuEngine {

    public class Entity : Object {
        public readonly ulong ID;

        public Entity() {
            ID = 0;
        }

        public T GetComponent<T>() where T : Component
        {
            if (typeof(T) == typeof(Transform))
                return (T)(object)new Transform() { Entity = this };

            Type type = typeof(T);
            IntPtr compPtr = InternalCalls.Entity_GetComponent(ID, type.TypeHandle.Value);

            GCHandle handle = GCHandle.FromIntPtr(compPtr);

            T comp = handle.Target as T;

            return comp;
        }

        public bool HasComponent<T>() where T : Component {
            if (typeof(T) == typeof(Transform))
                return true;

            Type type = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, type.TypeHandle.Value);
        }
    }
}