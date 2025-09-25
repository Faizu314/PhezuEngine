namespace PhezuEngine {
 
    public abstract class AssetRef
    {
        internal ulong Guid => m_Guid;
        private readonly ulong m_Guid;

        public AssetRef(ulong guid)
        {
            m_Guid = guid;
        }
        
        public override bool Equals(object obj)
        {
            return obj is AssetRef other && m_Guid.Equals(other.m_Guid);
        }

        public override int GetHashCode()
        {
            return m_Guid.GetHashCode();
        }
    }
}
