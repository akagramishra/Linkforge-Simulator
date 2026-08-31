#include "../include/render.hpp"
#include<cstddef>

namespace lf {

Vector2 ToScreen(Vec2d p, const View& v) {
    return { v.origin.x + (float)p.x * v.scale,
             v.origin.y - (float)p.y * v.scale }; 
}


Vec2d ToWorld(Vector2 s, const View& v) {
    return { (s.x - v.origin.x) / v.scale,
             (v.origin.y - s.y) / v.scale };
}
void DrawMechanism(const Pose& p, const View& v) {
    if (!p.valid) {
        DrawText("No assembly at this crank angle", 20, 60, 20, RED);
        return;
    }

    const Vector2 o2 = ToScreen(p.O2, v);
    const Vector2 a  = ToScreen(p.A,  v);
    const Vector2 b  = ToScreen(p.B,  v);
    const Vector2 o4 = ToScreen(p.O4, v);
    const Vector2 c = ToScreen(p.C, v);

    DrawLineEx(o2, o4, 2.0f, GRAY);      // ground
    DrawLineEx(o2, a,  5.0f, ORANGE);    // crank
    DrawLineEx(a,  b,  5.0f, SKYBLUE);   // coupler
    DrawLineEx(b,  o4, 5.0f, LIME);      // rocker

    DrawCircleV(o2, 7.0f, RAYWHITE);
    DrawCircleV(o4, 7.0f, RAYWHITE);
    DrawCircleV(a,  5.0f, RAYWHITE);
    DrawCircleV(b,  5.0f, RAYWHITE);


       
    DrawLineEx(a, c, 3.0f, Color{ 90, 130, 200, 255 });
    DrawLineEx(c, b, 3.0f, Color{ 90, 130, 200, 255 });
    DrawCircleV(c, 6.0f, Color{ 240, 120, 80, 255 });
}

void DrawCouplerCurve(const std::vector<lf::Vec2d> &couplerpoints, const View &v)
{
    if (couplerpoints.size() < 2) return;

    for (size_t i = 0; i < couplerpoints.size() - 1; ++i) {
        Vector2 p1 = ToScreen(couplerpoints[i], v);
        Vector2 p2 = ToScreen(couplerpoints[i + 1], v);
        DrawLineEx(p1, p2, 2.0f, Color{ 200, 200, 50, 255 });
    }
      DrawLineEx(ToScreen(couplerpoints.back(), v),
               ToScreen(couplerpoints.front(), v),
               2.0f, Color{ 200, 200, 50, 255 });
}
} 