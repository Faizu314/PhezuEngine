// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdint>
#include <functional>

namespace Phezu {
    
    struct GUID {
        GUID() = default;
        GUID(uint64_t v) : Value(v) {}
        uint64_t Value = 0;
        operator uint64_t() const { return Value; }
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
