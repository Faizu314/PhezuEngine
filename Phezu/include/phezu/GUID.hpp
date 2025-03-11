#pragma once

namespace Phezu {
    
    struct GUID {
        uint64_t Value = 0;
        operator uint64_t&() { return Value; }
        bool operator==(const GUID& other) const { return Value == other.Value; }
        bool operator<(const GUID& other) const { return Value < other.Value; }
    };
}

template <>
struct std::hash<Phezu::GUID> {
    std::size_t operator()(const Phezu::GUID& guid) const noexcept {
        return std::hash<uint64_t>()(guid.Value);
    }
};
