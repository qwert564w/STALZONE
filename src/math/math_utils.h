#pragma once
#include <cmath>
struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    float Length() const { return sqrtf(x*x + y*y + z*z); }
};
struct Matrix4x4 { float m[4][4]; };
namespace Math {
    bool WorldToScreen(Vec3 world, Vec3& screen, Matrix4x4 viewMatrix, int sw, int sh);
    Vec3 CalcAngle(Vec3 src, Vec3 dst);
}
