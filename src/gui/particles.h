#pragma once
#include <vector>
struct Particle { float x, y, vx, vy; };
namespace Particles {
    void Update();
    void Render();
}
