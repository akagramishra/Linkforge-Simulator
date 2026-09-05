#include "../include/ioui.hpp"
#include "../include/theme.hpp"
#include <algorithm>
#include <array>
#include <cmath>

namespace ioui {

void UserInput::Draw(lf::FourBar& fourBar) {
    const float margin = 16.0f;
    const float labelX = margin;
    const float boxX   = 150.0f;
    const float boxW   = kSidebarWidth - boxX - margin;
    const float boxH   = 28.0f;
    const float rowGap = 38.0f;
    float y = 46.0f;

    GuiPanel(Rectangle{ 0, 0, kSidebarWidth, (float)GetScreenHeight() }, "Link Lengths");

    theme::Text("Ground (r1)",  (int)labelX, (int)y + 6, theme::kBodySize, BLACK);
    if (GuiValueBox(Rectangle{ boxX, y, boxW, boxH }, nullptr, &groundLength, kMinLinkLength, kMaxLinkLength, editingGround)) editingGround = !editingGround;
    y += rowGap;

    theme::Text("Crank (r2)",   (int)labelX, (int)y + 6, theme::kBodySize, BLACK);
    if (GuiValueBox(Rectangle{ boxX, y, boxW, boxH }, nullptr, &crankLength, kMinLinkLength, kMaxLinkLength, editingCrank)) editingCrank = !editingCrank;
    y += rowGap;

    theme::Text("Coupler (r3)", (int)labelX, (int)y + 6, theme::kBodySize, BLACK);
    if (GuiValueBox(Rectangle{ boxX, y, boxW, boxH }, nullptr, &couplerLength, kMinLinkLength, kMaxLinkLength, editingCoupler)) editingCoupler = !editingCoupler;
    y += rowGap;

    theme::Text("Rocker (r4)",  (int)labelX, (int)y + 6, theme::kBodySize, BLACK);
    if (GuiValueBox(Rectangle{ boxX, y, boxW, boxH }, nullptr, &rockerLength, kMinLinkLength, kMaxLinkLength, editingRocker)) editingRocker = !editingRocker;

    
    // could briefly be negative/huge -- clamp our own copy before it reaches the solver.
    auto clampToValidLength = [](int length) { return std::clamp(length, kMinLinkLength, kMaxLinkLength); };

    fourBar.groundLength  = (double)clampToValidLength(groundLength);
    fourBar.crankLength   = (double)clampToValidLength(crankLength);
    fourBar.couplerLength = (double)clampToValidLength(couplerLength);
    fourBar.rockerLength  = (double)clampToValidLength(rockerLength);
}

void IOUI::Classify_mechanism(const lf::FourBar& fourBar) {
    struct Link { double length; int role; }; // role: 0=ground 1=crank 2=coupler 3=rocker
    std::array<Link, 4> links{ {
        { fourBar.groundLength, 0 }, { fourBar.crankLength, 1 }, { fourBar.couplerLength, 2 }, { fourBar.rockerLength, 3 }
    } };
    std::sort(links.begin(), links.end(), [](const Link& a, const Link& b) { return a.length < b.length; });

    shortestLength = links[0].length;
    otherLength1   = links[1].length;
    otherLength2   = links[2].length;
    longestLength  = links[3].length;

    const double shortPlusLong = shortestLength + longestLength;
    const double otherSum = otherLength1 + otherLength2;
    const double epsilon = 1e-6;

    if (std::fabs(shortPlusLong - otherSum) < epsilon) {
        grashofSatisfied = true; // borderline:=>> satisfies the condition with equality
        mechanismType = MechanismType::ChangePoint;
        return;
    }

    if (shortPlusLong < otherSum) {
        grashofSatisfied = true;
        switch (links[0].role) { // which physical link is shortest decides the type
            case 0:  mechanismType = MechanismType::DoubleCrank; break;
            case 1:
            case 3:  mechanismType = MechanismType::CrankRocker;  break;
            default: mechanismType = MechanismType::DoubleRocker; break;
        }
    } else {
        grashofSatisfied = false;
        mechanismType = MechanismType::TripleRocker;
    }
}

std::string IOUI::GetTypeName() const {
    switch (mechanismType) {
        case MechanismType::DoubleCrank:  return "Double Crank (Drag-Link)";
        case MechanismType::CrankRocker:  return "Crank-Rocker";
        case MechanismType::DoubleRocker: return "Double Rocker";
        case MechanismType::TripleRocker: return "Triple Rocker (Non-Grashof)";
        case MechanismType::ChangePoint:  return "Change-Point Mechanism";
    }
    return "Unknown";
}

void IOUI::Draw() const {
    const int x = 16;
    int y = 210;

    theme::Text(TextFormat("s (shortest)  = %.0f", shortestLength), x, y, theme::kBodySize, DARKGRAY); y += 22;
    theme::Text(TextFormat("l (longest)   = %.0f", longestLength),  x, y, theme::kBodySize, DARKGRAY); y += 22;
    theme::Text(TextFormat("p, q (others) = %.0f, %.0f", otherLength1, otherLength2), x, y, theme::kBodySize, DARKGRAY); y += 30;

    theme::Text(TextFormat("s + l = %.0f   p + q = %.0f", shortestLength + longestLength, otherLength1 + otherLength2), x, y, theme::kBodySize, DARKGRAY); y += 32;

    theme::Text(grashofSatisfied ? "Grashof condition satisfied" : "Grashof condition 'NOT' satisfied",
              x, y, theme::kBodySize, grashofSatisfied ? LIME : ORANGE); y += 30;

    theme::Text(TextFormat("Type: %s", GetTypeName().c_str()), x, y, theme::kHeadingSize, SKYBLUE);
}

}
