#include "FrameLab/SpecialRelativityModel.h"

#include <stdexcept>

#include "FrameLab/PhysicsConstants.h"

namespace framelab {

MichelsonMorleyResult SpecialRelativityModel::simulate(
    const MichelsonMorleyParameters& p) const {

    if (p.armLengthMeters <= 0.0) {
        throw std::invalid_argument("armLengthMeters must be positive");
    }
    if (p.wavelengthMeters <= 0.0) {
        throw std::invalid_argument("wavelengthMeters must be positive");
    }
    if (p.frameSpeedMetersPerSecond < 0.0 ||
        p.frameSpeedMetersPerSecond >= constants::SpeedOfLightMetersPerSecond) {
        throw std::invalid_argument("frameSpeedMetersPerSecond must satisfy 0 <= v < c");
    }

    // In the interferometer's rest frame, special relativity gives isotropic
    // light speed c. Equal arms therefore have equal round-trip travel time,
    // independent of apparatus orientation.
    const double roundTrip =
        2.0 * p.armLengthMeters / constants::SpeedOfLightMetersPerSecond;

    MichelsonMorleyResult result;
    result.modelName = "Special relativity";
    result.travelTimes.armASeconds = roundTrip;
    result.travelTimes.armBSeconds = roundTrip;
    result.timeDifferenceSeconds = 0.0;
    result.opticalPathDifferenceMeters = 0.0;
    result.fringeOffset = 0.0;
    result.ninetyDegreeRotationFringeShift = 0.0;
    return result;
}

} // namespace framelab
