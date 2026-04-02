using System;
using System.Runtime.InteropServices;

namespace PhezuEngine {

    public abstract class Component : Object {
        public Entity Entity { get; private set; }
        
        private void SetEntity(IntPtr ptr) {
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
    
    public class Rigidbody : Component {
        public Vector2 Velocity {
            get {
                InternalCalls.Rigidbody_GetVelocity(Entity.ID, out Vector2 velocity);
                return velocity;
            }

            set {
                InternalCalls.Rigidbody_SetVelocity(Entity.ID, ref value);
            }
        }
    }

    public class Renderer : Component
    {
        public Material Material {
            get
            {
                ulong materialID = InternalCalls.Renderer_GetMaterial(Entity.ID);
                return new Material(materialID);
            }
            set
            {
                InternalCalls.Renderer_SetMaterial(Entity.ID, value.ID);
            }
        }
    }

    public class CircleCollider : Component
    {
        public float Radius {
            get 
            {
                return InternalCalls.CircleCollider_GetRadius(Entity.ID);
            }
            set
            {
                InternalCalls.CircleCollider_SetRadius(Entity.ID, value);
            }
        }
    }

    public class BoxCollider : Component {
        public Vector2 Size {
            get {
                InternalCalls.BoxCollider_GetSize(Entity.ID, out Vector2 size);
                return size;
            }
            set {
                InternalCalls.BoxCollider_SetSize(Entity.ID, ref value);
            }
        }
    }

    public class PolygonCollider : Component
    {
        public Vector2[] Positions {
            get {
                return null;
            }
            set {

            }
        }
    }

    public abstract class BehaviourComponent : Component {
        
    }
}
