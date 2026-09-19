#pragma once

#include <string>

namespace framelab {

struct MichelsonMorleyParameters {
    // L is the one-way arm length used by the model, in meters.
    double armLengthMeters = 11.0;

    // Light wavelength used to convert optical path difference to fringes.
    double wavelengthMeters = 500e-9;

    // Hypothetical apparatus speed through the stationary ether.
    double frameSpeedMetersPerSecond = 29'780.0;

    // Angle between arm A and the hypothetical ether-wind direction.
    double orientationDegrees = 0.0;
};

struct ArmTravelTimes {
    double armASeconds = 0.0;
    double armBSeconds = 0.0;
};

struct MichelsonMorleyResult {
    std::string modelName;
    ArmTravelTimes travelTimes;

    // Signed difference: arm A round trip - arm B round trip.
    double timeDifferenceSeconds = 0.0;

    // c * delta_t, useful for displaying equivalent optical path difference.
    double opticalPathDifferenceMeters = 0.0;

    // Instantaneous phase/fringe offset between the two arms.
    double fringeOffset = 0.0;

    // Change in fringe offset if the whole apparatus is rotated 90 degrees.
    // This is the historically important Michelson-Morley comparison.
    double ninetyDegreeRotationFringeShift = 0.0;
};

struct ModelComparison {
    MichelsonMorleyResult first;
    MichelsonMorleyResult second;
};

} // namespace framelab
