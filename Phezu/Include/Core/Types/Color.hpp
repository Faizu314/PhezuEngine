// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

namespace Phezu {

    class Color {
    public:
        Color() = default;
        Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) {}
        uint8_t r = 255, g = 255, b = 255, a = 255;
    public:
        bool operator==(const Color& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
    public:
        static const Color Color::Clear;
        static const Color Color::White;
        static const Color Color::Green;
        static const Color Color::Red;
        static const Color Color::Black;
    };

    inline const Color Color::Clear = Color(0, 0, 0, 0);
    inline const Color Color::White = Color(255, 255, 255, 255);
    inline const Color Color::Green = Color(0, 255, 0, 255);
    inline const Color Color::Red = Color(255, 0, 0, 255);
    inline const Color Color::Black = Color(0, 0, 0, 255);
}