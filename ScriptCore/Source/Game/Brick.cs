using PhezuEngine;

namespace Game
{
    public class Brick : BehaviourComponent
    {
        private Transform m_Transform;
        private IBrickListener m_Listener;

        private void OnCreate()
        {
            m_Transform = Entity.GetComponent<Transform>();
        }
        
        public void Initialize(IBrickListener listener, Vector2 position)
        {
            m_Listener = listener;
            m_Transform.Position = position;
        }

        private void OnDestroy()
        {
            m_Listener.OnBrickDestroyed(this);
        }
    }
}