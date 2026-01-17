using System.Runtime.InteropServices;

namespace PhezuEngine {
 
    [StructLayout(LayoutKind.Sequential)]
    public struct PrefabRef : IAssetRef {
        
        public ulong Guid => m_Guid;
        public ulong Source => m_Source;
        private readonly ulong m_Guid;
        private readonly ulong m_Source;
        
        public PrefabRef(ulong guid, ulong source)
        {
            m_Guid = guid;
            m_Source = source;
        }
    }
    
}
