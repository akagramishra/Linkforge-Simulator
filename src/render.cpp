#include "../include/render.hpp"
#include "../include/theme.hpp"
#include<cstddef>

namespace lf {

Vector2 ToScreen(Vec2d worldPoint, const View& view) {
    return { view.origin.x + (float)worldPoint.x * view.scale,
             view.origin.y - (float)worldPoint.y * view.scale };
}

void DrawMechanism(const Pose& pose, const View& view) {
    if (!pose.valid) {
        const char* message = "No assembly at this crank angle";
        int textWidth = (int)MeasureTextEx(theme::HeadingFont(), message, theme::kHeadingSize, 1.0f).x;
        theme::Text(message, (int)(view.origin.x) - textWidth / 2, (int)(view.origin.y - 100), theme::kHeadingSize, RED);
        return;
    }

    const Vector2 inputPivot   = ToScreen(pose.inputPivot,   view);
    const Vector2 crankPin     = ToScreen(pose.crankPin,     view);
    const Vector2 rockerPin    = ToScreen(pose.rockerPin,    view);
    const Vector2 outputPivot  = ToScreen(pose.outputPivot,  view);
    const Vector2 couplerPoint = ToScreen(pose.couplerPoint, view);

    DrawLineEx(inputPivot, outputPivot, 2.0f, GRAY);      // ground
    DrawLineEx(inputPivot, crankPin,    5.0f, ORANGE);    // crank
    DrawLineEx(crankPin,   rockerPin,   5.0f, SKYBLUE);   // coupler
    DrawLineEx(rockerPin,  outputPivot, 5.0f, LIME);      // rocker

    DrawCircleV(inputPivot,  7.0f, RAYWHITE);
    DrawCircleV(outputPivot, 7.0f, RAYWHITE);
    DrawCircleV(crankPin,    5.0f, RAYWHITE);
    DrawCircleV(rockerPin,   5.0f, RAYWHITE);

    DrawLineEx(crankPin, couplerPoint, 3.0f, Color{ 90, 130, 200, 255 });
    DrawLineEx(couplerPoint, rockerPin, 3.0f, Color{ 90, 130, 200, 255 });
    DrawCircleV(couplerPoint, 6.0f, Color{ 240, 120, 80, 255 });
}

void DrawCouplerCurve(const std::vector<lf::Vec2d>& couplerPoints, const View& view)
{
    if (couplerPoints.size() < 2) return;

    for (size_t i = 0; i < couplerPoints.size() - 1; ++i) {
        Vector2 p1 = ToScreen(couplerPoints[i], view);
        Vector2 p2 = ToScreen(couplerPoints[i + 1], view);
        DrawLineEx(p1, p2, 2.0f, Color{ 200, 200, 50, 255 });
    }
    DrawLineEx(ToScreen(couplerPoints.back(), view),
               ToScreen(couplerPoints.front(), view),
               2.0f, Color{ 200, 200, 50, 255 });
}
}
