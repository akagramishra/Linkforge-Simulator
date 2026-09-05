#include<cmath>
#include "../include/solver.hpp"
#include<algorithm>
#include<vector>
#define PI 3.14159265358979323846

namespace lf{
    Pose Solve(const FourBar& fourBar){
        Pose pose{};
        const double ground  = fourBar.groundLength;
        const double crank   = fourBar.crankLength;
        const double coupler = fourBar.couplerLength;
        const double rocker  = fourBar.rockerLength;
        const double angle   = fourBar.crankAngle;

        pose.inputPivot  = { 0.0, 0.0 };
        pose.outputPivot = { ground, 0.0 };

        pose.crankPin.x = crank * cos(angle);
        pose.crankPin.y = crank * sin(angle);

        // coupler and rocker can only close the loop if the triangle inequality holds
        const double dx = pose.outputPivot.x - pose.crankPin.x;
        const double dy = pose.outputPivot.y - pose.crankPin.y;
        const double dist = sqrt(dx * dx + dy * dy);

        if (dist < 1e-9 || dist > (coupler + rocker) || dist < fabs(coupler - rocker)) {
            pose.valid = false; // no real assembly at this crank angle
            //thisis our error check for checking whether a four bar linkage is valid or not
            return pose;
        }

        const double a = (coupler * coupler - rocker * rocker + dist * dist) / (2.0 * dist);
        const double heightSquared = coupler * coupler - a * a;
        if (heightSquared < 0.0) {
            pose.valid = false; // rounding error pushed this just past real
            return pose;
        }
        const double height = sqrt(heightSquared);

        const double side = fourBar.crossed ? -1.0 : 1.0;

        pose.rockerPin.x = pose.crankPin.x + a * dx / dist + side * height * dy / dist;
        pose.rockerPin.y = pose.crankPin.y + a * dy / dist - side * height * dx / dist;

        const double cx = pose.rockerPin.x - pose.crankPin.x;
        const double cy = pose.rockerPin.y - pose.crankPin.y;
        const double couplerSpan = sqrt(cx * cx + cy * cy);
        if (couplerSpan < 1e-9) {
            pose.valid = false; // crankPin and rockerPin coincide
            return pose;
        }

        const double alongX = cx / couplerSpan, alongY = cy / couplerSpan;
        const double perpX  = -alongY,          perpY  = alongX;

        pose.couplerPoint.x = pose.crankPin.x + fourBar.couplerPointAlong * alongX + fourBar.couplerPointOffset * perpX;
        pose.couplerPoint.y = pose.crankPin.y + fourBar.couplerPointAlong * alongY + fourBar.couplerPointOffset * perpY;

        pose.valid = true;
        return pose;
    }

    bool IsGrashof(const FourBar& fourBar){
        double lengths[4] = { fourBar.groundLength, fourBar.crankLength, fourBar.couplerLength, fourBar.rockerLength };
        std::sort(lengths, lengths + 4);
        return lengths[0] + lengths[3] <= lengths[1] + lengths[2]; // shortest + longest <= the other two Grashof condition
    }

    std::vector<Vec2d> TraceCouplerCurve(FourBar fourBar, int steps)
    {
        std::vector<Vec2d> couplerPoints;
        couplerPoints.reserve(steps);
        for(int i=0; i<steps; ++i){
            fourBar.crankAngle = 2.0 * PI * i / steps;
            Pose pose = Solve(fourBar);
            if(pose.valid){
                couplerPoints.push_back(pose.couplerPoint);
            }
        }
        return couplerPoints;
    }
}
