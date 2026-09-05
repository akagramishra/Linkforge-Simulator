#include "../include/theme.hpp"



//Created for changung theme and frontend things from one place, singleton class to load and unload fonts

namespace theme {
namespace {
    Font bodyFont;
    Font headingFont;
    bool loaded = false;
}

void Load() {
    if (loaded) return;

    bodyFont    = LoadFontEx("C:\\Windows\\Fonts\\segoeui.ttf", kBodySize, nullptr, 0);
    headingFont = LoadFontEx("C:\\Windows\\Fonts\\segoeui.ttf", kHeadingSize, nullptr, 0);

    // fall back if that system font isn't found (e.g. non-Windows)
    if (bodyFont.texture.id == 0)    bodyFont = GetFontDefault();
    if (headingFont.texture.id == 0) headingFont = GetFontDefault();

    SetTextureFilter(bodyFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(headingFont.texture, TEXTURE_FILTER_BILINEAR);

    loaded = true;
}

void Unload() {
    if (!loaded) return;
    UnloadFont(bodyFont);
    UnloadFont(headingFont);
    loaded = false;
}

const Font& BodyFont()    { return bodyFont; }
const Font& HeadingFont() { return headingFont; }

void Text(const char* text, int x, int y, int fontSize, Color color) {
    const Font& font = (fontSize >= kHeadingSize) ? headingFont : bodyFont;
    DrawTextEx(font, text, { (float)x, (float)y }, (float)fontSize, 1.0f, color);
}

}
