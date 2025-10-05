using System.Runtime.InteropServices;

namespace PhezuEngine {
 
    [StructLayout(LayoutKind.Sequential)]
    public struct PrefabRef : IAssetRef {
        
        internal ulong Guid => m_Guid;
        private readonly ulong m_Guid;
        
        public PrefabRef(ulong guid)
        {
            m_Guid = guid;
        }
    }
    
}
