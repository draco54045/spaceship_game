#pragma once
#include <cmath>

struct Vec2 {
    float x = 0;
    float y = 0;
    Vec2() = default;
    Vec2(float _x, float _y): x(_x), y(_y) {}
    Vec2 operator+(const Vec2&o) const { return {x+o.x, y+o.y}; }
    Vec2 operator-(const Vec2&o) const { return {x-o.x, y-o.y}; }
    Vec2 operator*(float s) const { return {x*s, y*s}; }
};

inline float length(const Vec2 &v) { return std::sqrt(v.x*v.x + v.y*v.y); }
inline Vec2 normalize(const Vec2 &v) { float l = length(v); return l==0?Vec2(0,0):Vec2(v.x/l, v.y/l); }
