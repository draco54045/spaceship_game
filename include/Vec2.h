#pragma once
#include <cmath>
#include <iostream>

struct Vec2 {
    float x = 0;
    float y = 0;
    Vec2() = default;
    Vec2(float _x, float _y): x(_x), y(_y) {}
    Vec2 operator+(const Vec2&o) const { return {x+o.x, y+o.y}; }
    Vec2 operator-(const Vec2&o) const { return {x-o.x, y-o.y}; }
    Vec2 operator*(float s) const { return {x*s, y*s}; }
    Vec2 operator/(float s) const { return {x/s, y/s}; }
};
template<typename T>
T clamp(T val, T mn, T mx){
return std::max(std::min(val, mx), mn);
}

inline float length(const Vec2 &v) { return std::sqrt(v.x*v.x + v.y*v.y); }
inline Vec2 normalize(const Vec2 &v) { float l = length(v); return l==0?Vec2(0,0):Vec2(v.x/l, v.y/l); }
inline float dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }
inline Vec2 lerpVec(const Vec2& a, const Vec2& b, float t) { return a + (b - a) * clamp(t, 0.0f, 1.0f);}
inline float lerpAngle(float a, float b, float t) { float diff = fmodf(b - a + 540.0f, 360.0f) - 180.0f; return a + diff * clamp(t, 0.0f, 1.0f);}
