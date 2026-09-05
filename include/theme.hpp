#pragma once
#include "raylib.h"

namespace theme {

    constexpr int kBodySize    = 16; 
    constexpr int kHeadingSize = 20; 

    void Load();
    void Unload();

    const Font& BodyFont();
    const Font& HeadingFont();

    void Text(const char* text, int x, int y, int fontSize, Color color);
}
