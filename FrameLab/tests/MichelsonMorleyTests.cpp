#include <cmath>
#include <cstdlib>
#include <iostream>

#include "FrameLab/ClassicalEtherModel.h"
#include "FrameLab/PhysicsConstants.h"
#include "FrameLab/SpecialRelativityModel.h"

namespace {
void require(const bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

bool near(const double a, const double b, const double tolerance) {
    return std::abs(a - b) <= tolerance;
}
}

int main() {
    using namespace framelab;

    MichelsonMorleyParameters p;
    p.armLengthMeters = 11.0;
    p.wavelengthMeters = 500e-9;
    p.frameSpeedMetersPerSecond = constants::EarthOrbitalSpeedMetersPerSecond;
    p.orientationDegrees = 0.0;

    ClassicalEtherModel ether;
    SpecialRelativityModel sr;

    const auto e0 = ether.simulate(p);
    const auto s0 = sr.simulate(p);

    // Historical-order check: the old ether model predicts roughly four-tenths
    // of a fringe on a 90-degree rotation for these Michelson-Morley-like values.
    require(std::abs(e0.ninetyDegreeRotationFringeShift) > 0.35,
            "ether prediction should exceed 0.35 fringe");
    require(std::abs(e0.ninetyDegreeRotationFringeShift) < 0.50,
            "ether prediction should be below 0.50 fringe");

    require(near(s0.timeDifferenceSeconds, 0.0, 1e-30),
            "SR should predict zero arm time difference");
    require(near(s0.ninetyDegreeRotationFringeShift, 0.0, 1e-30),
            "SR should predict zero rotation fringe shift");

    // At 45 degrees, the two perpendicular arms are symmetric relative to the
    // ether-wind direction, so the instantaneous fringe offset is zero.
    p.orientationDegrees = 45.0;
    const auto e45 = ether.simulate(p);
    require(std::abs(e45.fringeOffset) < 1e-7,
            "ether model should have ~zero instantaneous offset at 45 degrees");

    // With v=0, both theories agree.
    p.orientationDegrees = 0.0;
    p.frameSpeedMetersPerSecond = 0.0;
    const auto eRest = ether.simulate(p);
    require(near(eRest.timeDifferenceSeconds, 0.0, 1e-30),
            "ether model should reduce to equal arm times at v=0");

    std::cout << "All Michelson-Morley tests passed.\n";
    return EXIT_SUCCESS;
}
