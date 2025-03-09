#include <vector>
#include <string>

namespace Phezu {
    
    class Json {
    public:
        template<typename T>
        static std::string SerializeArray(std::vector<T> array) {
            
        }
        
        template<typename T>
        static std::vector<T> DeserializeArray(std::string data) {
            
        }
        
        template<typename T>
        static std::string SerializeRaw(T item) {
            
        }
        
        template<typename T>
        static T DeserializeRaw(std::string data) {
            
        }
    };
}
