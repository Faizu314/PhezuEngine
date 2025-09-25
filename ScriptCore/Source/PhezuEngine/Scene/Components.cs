using System;
using System.Runtime.InteropServices;

namespace PhezuEngine {

    public class Component : Object {
        public Entity Entity { get; internal set; }
        
        internal void SetEntity(IntPtr ptr) {
            GCHandle handle = GCHandle.FromIntPtr(ptr);

            Entity = handle.Target as Entity;
        }
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
    
    public class Physics : Component {
        public Vector2 Velocity {
            get {
                InternalCalls.Physics_GetVelocity(Entity.ID, out Vector2 velocity);
                return velocity;
            }

            set {
                InternalCalls.Physics_SetVelocity(Entity.ID, ref value);
            }
        }
    
    }

    public abstract class BehaviourComponent : Component {
        
    }
}
