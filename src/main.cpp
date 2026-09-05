#include "raylib.h"
#include "../include/solver.hpp"
#include "../include/render.hpp"
#include "../include/ioui.hpp"
#include "../include/theme.hpp"
#include <algorithm>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720
#define MIN_WINDOW_WIDTH 760
#define MIN_WINDOW_HEIGHT 640

struct WorldBounds {
    float minX, maxX, minY, maxY;
};

static WorldBounds ComputeWorldBounds(const lf::FourBar& fourBar, const std::vector<lf::Vec2d>& couplerCurve) {
    float minX = 0.0f, maxX = (float)fourBar.groundLength;
    float minY = 0.0f, maxY = 0.0f;

    minX = std::min(minX, -(float)fourBar.crankLength);
    maxX = std::max(maxX,  (float)fourBar.crankLength);
    minY = std::min(minY, -(float)fourBar.crankLength);
    maxY = std::max(maxY,  (float)fourBar.crankLength);

    minX = std::min(minX, (float)fourBar.groundLength - (float)fourBar.rockerLength);
    maxX = std::max(maxX, (float)fourBar.groundLength + (float)fourBar.rockerLength);
    minY = std::min(minY, -(float)fourBar.rockerLength);
    maxY = std::max(maxY,  (float)fourBar.rockerLength);

    for (const lf::Vec2d& point : couplerCurve) {
        minX = std::min(minX, (float)point.x);
        maxX = std::max(maxX, (float)point.x);
        minY = std::min(minY, (float)point.y);
        maxY = std::max(maxY, (float)point.y);
    }

    return { minX, maxX, minY, maxY };
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Linkforge Simulator");
    SetWindowMinSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    SetTargetFPS(60);

    theme::Load();
    GuiSetFont(theme::BodyFont());
    GuiSetStyle(DEFAULT, TEXT_SIZE, theme::kBodySize);

    lf::FourBar fourBar;
    lf::View view;
    ioui::UserInput userInput;
    ioui::IOUI classifier;
    std::vector<lf::Vec2d> couplerCurve = lf::TraceCouplerCurve(fourBar);
    WorldBounds worldBounds = ComputeWorldBounds(fourBar, couplerCurve);

    double crankAngularSpeed = 1.5; // rad/s
    bool running = true;
    lf::FourBar lastFourBar = fourBar; // to detect a length change and retrace the curve

    while (!WindowShouldClose()) {
        // input
        if (IsKeyPressed(KEY_SPACE)) {
            fourBar.crossed = !fourBar.crossed;
            couplerCurve = lf::TraceCouplerCurve(fourBar);
            worldBounds = ComputeWorldBounds(fourBar, couplerCurve);
        }
        if (IsKeyPressed(KEY_P)) running = !running;
        if (running) fourBar.crankAngle += crankAngularSpeed * GetFrameTime();

        // update
        lf::Pose pose = lf::Solve(fourBar);

        // fit worldBounds into the drawing area, centered, with a small margin
        float availableWidth  = (float)GetScreenWidth() - ioui::kSidebarWidth;
        float availableHeight = (float)GetScreenHeight();
        float shapeWidth  = std::max(worldBounds.maxX - worldBounds.minX, 1.0f);
        float shapeHeight = std::max(worldBounds.maxY - worldBounds.minY, 1.0f);
        float shapeCenterX = (worldBounds.minX + worldBounds.maxX) * 0.5f;
        float shapeCenterY = (worldBounds.minY + worldBounds.maxY) * 0.5f;
        const float fitMargin = 0.85f;

        view.scale = std::clamp(std::min(availableWidth / shapeWidth, availableHeight / shapeHeight) * fitMargin, 0.2f, 3.0f);
        view.origin = { ioui::kSidebarWidth + availableWidth * 0.5f - shapeCenterX * view.scale,
                         availableHeight * 0.5f + shapeCenterY * view.scale };

        // draw
        BeginDrawing();
        ClearBackground(Color{ 24, 24, 28, 255 });

        lf::DrawCouplerCurve(couplerCurve, view);
        lf::DrawMechanism(pose, view);

        userInput.Draw(fourBar);
        bool lengthsChanged = fourBar.groundLength  != lastFourBar.groundLength
                           || fourBar.crankLength   != lastFourBar.crankLength
                           || fourBar.couplerLength != lastFourBar.couplerLength
                           || fourBar.rockerLength  != lastFourBar.rockerLength;
        if (lengthsChanged) {
            couplerCurve = lf::TraceCouplerCurve(fourBar);
            worldBounds = ComputeWorldBounds(fourBar, couplerCurve);
            lastFourBar = fourBar;
        }

        classifier.Classify_mechanism(fourBar);
        classifier.Draw();

        theme::Text(TextFormat("crossed: %s", fourBar.crossed ? "true" : "false"), 16, GetScreenHeight() - 70, theme::kBodySize, MAROON);
        theme::Text(running ? "running (P to pause)" : "paused (P to run)", 16, GetScreenHeight() - 50, theme::kBodySize, MAROON);
        theme::Text("SPACE flip circuit", 16, GetScreenHeight() - 28, theme::kBodySize, DARKGRAY);
        EndDrawing();
    }

    theme::Unload();
    CloseWindow();
    return 0;
}
