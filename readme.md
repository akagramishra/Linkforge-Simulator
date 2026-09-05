# Linkforge Simulator

A four-bar linkage simulator: type in four link lengths, watch the mechanism
move, and see whether it's a crank-rocker, double-crank, double-rocker,
triple-rocker, or change-point mechanism per Grashof's law.

## How data flows

```
UserInput (ioui)         IOUI (ioui)
   |  writes lengths        |  reads lengths
   v                        v
        lf::FourBar  (mechanism.hpp)
                |
                |  lf::Solve() / lf::TraceCouplerCurve()   (solver.cpp)
                v
        lf::Pose  (mechanism.hpp)
                |
                |  lf::DrawMechanism() / lf::DrawCouplerCurve()   (render.cpp)
                v
             screen
```

`main.cpp` owns the loop: each frame it reads input, lets `UserInput` update
the `FourBar`, solves it, fits the view to whatever the window size is, draws
it, then hands the same `FourBar` to `IOUI` to classify and report.

## Files

| File | What's in it | Depends on |
|---|---|---|
| [vec2.hpp](include/vec2.hpp) | `Vec2d` -- a plain 2D point | nothing |
| [mechanism.hpp](include/mechanism.hpp) | `FourBar` (the four lengths + current angle) and `Pose` (the solved joint positions). Pure data, no behaviour. | vec2.hpp |
| [solver.hpp](include/solver.hpp) / [solver.cpp](src/solver.cpp) | The kinematics: `Solve()` turns a `FourBar` into a `Pose`; `IsGrashof()` checks Grashof's law; `TraceCouplerCurve()` sweeps a full rotation to trace the coupler curve | mechanism.hpp |
| [render.hpp](include/render.hpp) / [render.cpp](src/render.cpp) | Turns a `Pose` into on-screen lines/circles (`DrawMechanism`) and draws the traced curve (`DrawCouplerCurve`). Knows nothing about the sidebar. | mechanism.hpp, theme.hpp, raylib |
| [ioui.hpp](include/ioui.hpp) / [ioui.cpp](src/ioui.cpp) | The left-hand sidebar: `UserInput` draws the four length boxes and writes them into a `FourBar`; `IOUI` classifies a `FourBar` and draws the Grashof readout + mechanism type | solver.hpp, theme.hpp, raygui.hpp |
| [theme.hpp](include/theme.hpp) / [theme.cpp](src/theme.cpp) | Loads two fonts baked at the exact sizes the app draws (`kBodySize`/`kHeadingSize`), so text is crisp instead of a stretched bitmap font; `theme::Text()` is what every on-screen label goes through | raylib |
| [main.cpp](src/main.cpp) | The window and the per-frame loop that wires everything above together, plus fitting the view to the window as it's resized | all of the above |
| [raygui.hpp](include/raygui.hpp) / [raygui_impl.cpp](src/raygui_impl.cpp) | Third-party immediate-mode GUI library (unmodified) -- gives us `GuiValueBox`, `GuiPanel`, etc. `raygui_impl.cpp` is the one file that defines `RAYGUI_IMPLEMENTATION`. | raylib |

## Namespaces

- `lf` -- the linkage itself: data (`mechanism.hpp`), math (`solver.hpp`), drawing (`render.hpp`). Nothing in here knows a GUI exists.
- `ioui` -- the input/output panel that sits on top: reading numbers in, reporting the classification out.
- `theme` -- shared text styling, used by both of the above.

## Naming conventions

Every `FourBar` field is named after its kinematic role, not a generic
`r1`/`r2`/`r3`/`r4`: `groundLength`, `crankLength`, `couplerLength`,
`rockerLength`. The letters `s`, `l`, `p`, `q` are reserved for Grashof's
law itself (shortest, longest, and the other two lengths, in `ioui.hpp`) --
they're computed by sorting the four lengths, not tied to any one link, so
they're kept separate from the role names to avoid implying a link is
shortest just because of which box it was typed into.

## Input safety

Link lengths are clamped to `[kMinLinkLength, kMaxLinkLength]` (10 to 1000,
`include/ioui.hpp`) before they ever reach the solver, so a stray minus
sign or an oversized number typed mid-edit can't produce a broken pose.
`Solve()` (`src/solver.cpp`) additionally checks for degenerate geometry
(coincident pivots, a triangle that doesn't close) and reports `Pose::valid
= false` instead of silently producing `NaN` joint positions.

## Window and view

The window is resizable (`FLAG_WINDOW_RESIZABLE`, with a minimum size set
so the sidebar always fits). Each frame, `main.cpp` computes a `WorldBounds`
covering everywhere the mechanism can reach over a full rotation -- both
pivot circles plus the traced coupler curve -- and centers the view on
that, scaled to fit the current window. That's what keeps the simulation
centered and correctly sized no matter the window size or the lengths typed in.
