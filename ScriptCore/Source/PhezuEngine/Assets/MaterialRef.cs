using System.Runtime.InteropServices;

namespace PhezuEngine {
 
    [StructLayout(LayoutKind.Sequential)]
    public struct MaterialRef : IAssetRef {
        
        public ulong Guid => m_Guid;
        private readonly ulong m_Guid;
        
        public MaterialRef(ulong guid)
        {
            m_Guid = guid;
        }
    }
    
}
