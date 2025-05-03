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

            return null;
        }
    }
}