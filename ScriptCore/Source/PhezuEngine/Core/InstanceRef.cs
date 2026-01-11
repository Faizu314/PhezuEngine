#pragma warning disable CS0649

using System;
using System.Runtime.InteropServices;

namespace PhezuEngine
{
    public struct InstanceRef<T> where T : Object
    {
        private IntPtr m_InstanceHandle;

        public T Get()
        {
            GCHandle handle = GCHandle.FromIntPtr(m_InstanceHandle);
            
            return (T)handle.Target;
        }
    }
}