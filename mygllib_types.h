#pragma once

#include <cstring>
#include <memory>
#include <string>

namespace mygl {

using uint = unsigned int;
using byte = unsigned char;

template <typename T>
struct Vec2 {
    T x;
    T y;

    Vec2(T _x, T _y) {
        x = _x;
        y = _y;
    }

    Vec2() {
        x = -1;
        y = -1;
    }
};

using Point2D = Vec2<int>;

struct RGBA {
    // b,g,r,a is ordered, Suiting for the Win32 Draw API, DO NOT change.
    byte m_b;  // blue
    byte m_g;  // gree
    byte m_r;  // red
    byte m_a;  // alpha

    RGBA(byte r = 255, byte g = 255, byte b = 255, byte a = 255)
        : m_r(r), m_g(g), m_b(b), m_a(a) {}
};

}  // namespace mygl
