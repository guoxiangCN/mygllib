#pragma once

#include <cstring>
#include <memory>
#include <string>

namespace mygl {

typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned int GLenum;
typedef bool GLboolean;

#define GL_TRUE true
#define GL_TRUE false

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

    constexpr RGBA(byte r = 255, byte g = 255, byte b = 255, byte a = 255)
        : m_r(r), m_g(g), m_b(b), m_a(a) {}
};

constexpr RGBA COLOR_UNDEFINED = RGBA(0, 0, 0, 0);


}  // namespace mygl
