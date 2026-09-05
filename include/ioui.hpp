#pragma once
#include "solver.hpp"
#include "raygui.hpp"
#include <string>

namespace ioui {

    constexpr float kSidebarWidth = 280.0f;

    // clamped so a stray keystroke (negative sign, empty box, huge number) can't reach the solver
    constexpr int kMinLinkLength = 10;
    constexpr int kMaxLinkLength = 1000;

    enum class MechanismType {
        DoubleCrank,
        CrankRocker,
        DoubleRocker,
        TripleRocker,
        ChangePoint
    };

    // Boxes are labelled by kinematic role (ground/crank/coupler/rocker), not
    // by Grashof's s/l/p/q -- which link is shortest depends on the numbers
    // typed in, not on which box they went into.
    class UserInput {
        public:
            void Draw(lf::FourBar& fourBar);

        private:
            int groundLength  = 200;
            int crankLength   = 60;
            int couplerLength = 180;
            int rockerLength  = 120;

            bool editingGround = false, editingCrank = false, editingCoupler = false, editingRocker = false;
    };

    // Grashof's law: shortest + longest <= the other two.
    class IOUI {
        public:
            void Classify_mechanism(const lf::FourBar& fourBar);
            void Draw() const;

            bool IsGrashof() const { return grashofSatisfied; }
            MechanismType GetType() const { return mechanismType; }
            std::string GetTypeName() const;

        private:
            MechanismType mechanismType = MechanismType::TripleRocker;
            bool grashofSatisfied = false;

            double shortestLength = 0.0;
            double longestLength  = 0.0;
            double otherLength1   = 0.0;
            double otherLength2   = 0.0;
    };

}
