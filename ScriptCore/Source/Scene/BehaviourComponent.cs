

namespace PhezuEngine {
 
    public class Component {
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
    }
}