

namespace PhezuEngine {
    
    public class Material
    {
        public readonly ulong ID;

        internal Material(ulong id)
        {
            ID = id;
        }

        public Color Tint
        {
            get
            {
                InternalCalls.Material_GetColor(ID, "tint", out var color);
                return color;
            }
            set
            {
                InternalCalls.Material_SetColor(ID, "tint", ref value);
            }
        }

        public Color GetColor(string propertyName)
        {
            InternalCalls.Material_GetColor(ID, propertyName, out var color);
            return color;
        }

        public void SetColor(string propertyName, Color color)
        {
            InternalCalls.Material_SetColor(ID, propertyName, ref color);
        }

        public static Material Create(Material source)
        {
            return new Material(InternalCalls.Material_Create(source.ID));
        }

        public static Material GetMaterial(MaterialRef materialRef)
        {
            return new Material(InternalCalls.Material_Get(materialRef.Guid, materialRef.Source));
        }
    }

}