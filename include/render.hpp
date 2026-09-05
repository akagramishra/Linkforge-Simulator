#pragma once
#include "raylib.h"
#include<vector>
#include "mechanism.hpp"

namespace lf {

struct View {
    Vector2 origin = { 250.0f, 420.0f }; // where world (0,0) sits on screen, in pixels
    float scale = 1.6f;                  // pixels per world unit
};

Vector2 ToScreen(Vec2d worldPoint, const View& view);
void DrawMechanism(const Pose& pose, const View& view);

void DrawCouplerCurve(const std::vector<lf::Vec2d>& couplerPoints, const View& view);
}
