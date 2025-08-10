using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PhezuEngine {

    public class Entity : Object {
        public readonly ulong ID;
        
        private Dictionary<Type, Component> m_ComponentCache;

        public Entity() {
            ID = 0;
            m_ComponentCache = new();
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
            
            if (m_ComponentCache.ContainsKey(type))
                return (T)(object)m_ComponentCache[type];
            
            if (type == typeof(Transform))
                m_ComponentCache[type] = new Transform() { Entity = this };
            else if (type == typeof(Physics))
                m_ComponentCache[type] = new Physics() { Entity = this };
            else {
                IntPtr compPtr = InternalCalls.Entity_GetComponent(ID, type.TypeHandle.Value);

                GCHandle handle = GCHandle.FromIntPtr(compPtr);
                
                m_ComponentCache[type] = handle.Target as Component;
            }
            
            return (T)m_ComponentCache[type];
        }

        public bool HasComponent<T>() where T : Component {
            if (typeof(T) == typeof(Transform))
                return true;

            Type type = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, type.TypeHandle.Value);
        }
    }
}
