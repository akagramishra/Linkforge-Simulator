#include<cmath>
#include "../include/solver.hpp"
#include<algorithm>
#include<vector>
#define PI 3.14159265358979323846

namespace lf{
    Pose Solve(const FourBar& m){
        Pose p{};
        double r1 = m.r1;
        double r2 = m.r2;
        double r3 = m.r3;
        double r4 = m.r4;
        double theta2 = m.theta2;

        
        p.A.x = r2 * cos(theta2);
        p.A.y = r2 * sin(theta2);

        
        p.O4.x = r1;
        p.O4.y = 0.0;

        
        double d = sqrt(pow(p.A.x - p.O4.x, 2) + pow(p.A.y - p.O4.y, 2));
        if (d > (r3 + r4) || d < fabs(r3 - r4)) {
            p.valid = false; // No solution exists
            return p;
        }

        double a = (pow(r3, 2) - pow(r4, 2) + pow(d, 2)) / (2 * d);
        double h = sqrt(pow(r3, 2) - pow(a, 2));

        double s  = m.crossed ? -1.0 : 1.0; 

        // Calculate the coordinates of point B
        p.B.x = p.A.x + a * (p.O4.x - p.A.x) / d + s * h * (p.O4.y - p.A.y) / d;
        p.B.y = p.A.y + a * (p.O4.y - p.A.y) / d - s * h * (p.O4.x - p.A.x) / d;


       
        p.O2.x = 0.0;
        p.O2.y = 0.0;

        double bx = p.B.x - p.A.x;
        double by = p.B.y - p.A.y;
        double L  = sqrt(bx * bx + by * by);

        double ux = bx / L,  uy = by / L;   
        double nx = -uy,     ny = ux;    

        p.C.x = p.A.x + m.cp_a * ux + m.cp_b * nx;
        p.C.y = p.A.y + m.cp_a * uy + m.cp_b * ny;

        p.valid = true; 
        return p;
    }
     bool IsGrashof(const FourBar& m){
          double L[4] = { m.r1, m.r2, m.r3, m.r4 };
    std::sort(L, L + 4);
    return L[0] + L[3] <= L[1] + L[2];   // s + l ≤ p + q
     }

     std::vector<Vec2d> TraceCouplerCurve(FourBar m, int steps)
     {
        std::vector<Vec2d> couplerPoints;
        couplerPoints.reserve(steps);
        for(int i=0; i<steps; ++i){
            m.theta2 = 2.0 * PI * i / steps;
            Pose p = Solve(m);
            if(p.valid){
                
                couplerPoints.push_back(p.C);// this is what gives the point for making the coupler curve
                
            }
     }
     return couplerPoints;
    }
}



