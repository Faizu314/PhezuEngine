using System;
using System.Runtime.InteropServices;

namespace PhezuEngine {
 
    public class Object {
        private GCHandle m_GcHandle;

        public Object() {
            m_GcHandle = GCHandle.Alloc(this, GCHandleType.Normal);
        }

        public IntPtr GetGcHandle() {
            return GCHandle.ToIntPtr(m_GcHandle);
        }
    }

    public class Component : Object {
        public Entity Entity { get; internal set; }
    }

    public class Transform : Component {
        public Vector2 Position {
            get {
                InternalCalls.Transform_GetPosition(Entity.ID, out Vector2 position);
                return position;
            }

            set {
                InternalCalls.Transform_SetPosition(Entity.ID, ref value);
            }
        }
    }

    public abstract class BehaviourComponent : Component {
        public abstract void OnCreated();
        public abstract void OnDestroyed();

        public abstract void OnUpdate(float deltaTime);

        internal void SetEntity(IntPtr ptr) {
            GCHandle handle = GCHandle.FromIntPtr(ptr);

            Entity = handle.Target as Entity;
            Console.WriteLine("BehaviourComponent.SetEntity | Entity: " + Entity + ", ID: " + Entity.ID);
        }
    }
}