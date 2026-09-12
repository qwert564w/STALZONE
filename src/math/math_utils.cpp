#include "math_utils.h"
namespace Math {
    bool WorldToScreen(Vec3 world, Vec3& screen, Matrix4x4 viewMatrix, int sw, int sh) {
        float w = viewMatrix.m[3][0] * world.x + viewMatrix.m[3][1] * world.y + viewMatrix.m[3][2] * world.z + viewMatrix.m[3][3];
        if (w < 0.01f) return false;
        float invW = 1.0f / w;
        screen.x = (viewMatrix.m[0][0] * world.x + viewMatrix.m[0][1] * world.y + viewMatrix.m[0][2] * world.z + viewMatrix.m[0][3]) * invW;
        screen.y = (viewMatrix.m[1][0] * world.x + viewMatrix.m[1][1] * world.y + viewMatrix.m[1][2] * world.z + viewMatrix.m[1][3]) * invW;
        screen.x = (screen.x + 1.0f) * 0.5f * sw;
        screen.y = (1.0f - (screen.y + 1.0f) * 0.5f) * sh;
        return true;
    }
    Vec3 CalcAngle(Vec3 src, Vec3 dst) {
        Vec3 angle; Vec3 delta = src - dst;
        float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);
        angle.x = (float)(atan(delta.z / hyp) * 57.295779513082f);
        angle.y = (float)(atan(delta.y / delta.x) * 57.295779513082f);
        if (delta.x >= 0.0f) angle.y += 180.0f;
        angle.z = 0.0f; return angle;
    }
}
