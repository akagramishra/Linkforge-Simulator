#pragma once
#include "raylib.h"
#include<vector>
#include "mechanism.hpp"

namespace lf {

struct View {
    Vector2 origin = { 250.0f, 420.0f };  // where world (0,0) sits on screen
    float scale = 1.6f;                   // pixels per mm
};

Vector2 ToScreen(Vec2d p, const View& v);
void DrawMechanism(const Pose& p, const View& v);

void DrawCouplerCurve(const std::vector<lf::Vec2d>& couplerpoints, const View& v);
}