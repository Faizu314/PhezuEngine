using System;
using System.Runtime.InteropServices;

namespace PhezuEngine {

    public class Entity : Object {
        public readonly ulong ID;
        
        public Entity() {
            ID = 0;
        }
        
        public static Entity GetEntity(IntPtr entityRef) {
            GCHandle handle = GCHandle.FromIntPtr(entityRef);

            var entity = handle.Target as Entity;
            
            return entity;
        }
        
        public string Tag {
            get {
                return InternalCalls.Entity_GetTag(ID);
            }
        }

        public T GetComponent<T>() where T : Component
        {
            Type type = typeof(T);
            
            IntPtr compPtr = InternalCalls.Entity_GetComponent(ID, type.TypeHandle.Value);

            GCHandle handle = GCHandle.FromIntPtr(compPtr);
            
            return (T)handle.Target;
        }

        public bool HasComponent<T>() where T : Component {
            if (typeof(T) == typeof(Transform))
                return true;

            Type type = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, type.TypeHandle.Value);
        }

        public void Destroy()
        {
            InternalCalls.Entity_Destroy(ID);
        }
    }
}
