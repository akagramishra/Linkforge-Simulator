#pragma once
#include "vec2.hpp"

namespace lf {
    struct FourBar {
        Vec2d position;
        double r1= 200.0;
        double r2= 60.0;
        double r3= 180.0;
        double r4= 120.0;

        double cp_a = 90.0;//dist along A and B
        double cp_b = 70.0;// perpen offset

        double theta2= 0.0;
        bool crossed = false;
    };

    struct Pose{
        Vec2d O2, A, B, O4;
        bool valid = false;
        Vec2d C;
    };
}