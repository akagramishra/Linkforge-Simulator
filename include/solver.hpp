#pragma once
#include "mechanism.hpp"
#include <vector>

namespace lf {
    Pose Solve(const FourBar& fourBar);
    bool IsGrashof(const FourBar& fourBar);
    std::vector<Vec2d> TraceCouplerCurve(FourBar fourBar, int steps = 720);
}
