#pragma once
#include "vec2.hpp"

namespace lf {

    struct FourBar {
        double groundLength  = 200.0;
        double crankLength   = 60.0;
        double couplerLength = 180.0;
        double rockerLength  = 120.0;

        double couplerPointAlong  = 90.0;
        double couplerPointOffset = 70.0;

        double crankAngle = 0.0;
        bool crossed = false; // a four-bar has two valid assemblies; picks which one
    };

    struct Pose {
        Vec2d inputPivot;
        Vec2d crankPin;
        Vec2d rockerPin;
        Vec2d outputPivot;
        Vec2d couplerPoint;
        bool valid = false; // false if no real assembly exists at this angle
    };
}
