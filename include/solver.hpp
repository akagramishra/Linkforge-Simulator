#pragma once
#include "mechanism.hpp"
#include <vector>

namespace lf {
    Pose Solve(const FourBar& m);
    bool IsGrashof(const FourBar& m);
    std::vector<Vec2d> TraceCouplerCurve(FourBar m, int steps = 720);
}