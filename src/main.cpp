#include "raylib.h"
#include "../include/solver.hpp"
#include "../include/render.hpp"
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 800

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Linkforge Simulator");
    SetTargetFPS(60);

    lf::FourBar m;
    lf::View view;
    std::vector<lf::Vec2d> curve = lf::TraceCouplerCurve(m);
    
    double omega = 1.5;      // rad/s
    bool running = true;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) m.crossed = !m.crossed, curve = lf::TraceCouplerCurve(m);
        if (IsKeyPressed(KEY_P))     running = !running;
        if (running) m.theta2 += omega * GetFrameTime();

        lf::Pose p = lf::Solve(m);

        BeginDrawing();
        ClearBackground(Color{ 24, 24, 28, 255 });
        
       
        lf::DrawCouplerCurve(curve, view);
        lf::DrawMechanism(p, view);
        DrawText(lf::IsGrashof(m) ? "Grashof" : "non-Grashof", 20, 20, 18, GRAY);
        DrawText(TextFormat("crossed: %s", m.crossed ? "true" : "false"), 20, 45, 18, YELLOW);
        DrawText("SPACE flip circuit   P pause", 20, 570, 16, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}