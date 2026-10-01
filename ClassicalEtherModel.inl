#include "FrameLab/ClassicalEtherModel.h"

#include <cmath>
#include <stdexcept>

#include "FrameLab/PhysicsConstants.h"

namespace framelab {

namespace {
void validate(const MichelsonMorleyParameters& p) {
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
}
}

double ClassicalEtherModel::roundTripTimeSeconds(
    const double armLengthMeters,
    const double frameSpeedMetersPerSecond,
    const double armAngleRadians) {

    const double c = constants::SpeedOfLightMetersPerSecond;
    const double v = frameSpeedMetersPerSecond;
    const double sinTheta = std::sin(armAngleRadians);

    // Exact Galilean stationary-ether prediction for a moving interferometer arm:
    // t(theta) = 2 L sqrt(c^2 - v^2 sin^2(theta)) / (c^2 - v^2)
    //
    // theta = 0:     2Lc/(c^2-v^2)  (parallel arm)
    // theta = pi/2:  2L/sqrt(c^2-v^2) (transverse arm)
    const double denominator = c * c - v * v;
    const double insideRoot = c * c - v * v * sinTheta * sinTheta;

    return (2.0 * armLengthMeters * std::sqrt(insideRoot)) / denominator;
}

MichelsonMorleyResult ClassicalEtherModel::simulate(
    const MichelsonMorleyParameters& p) const {

    validate(p);

    const double theta = p.orientationDegrees * constants::DegreesToRadians;
    const double armBTheta = theta + 90.0 * constants::DegreesToRadians;

    MichelsonMorleyResult result;
    result.modelName = "Classical stationary ether";
    result.travelTimes.armASeconds = roundTripTimeSeconds(
        p.armLengthMeters, p.frameSpeedMetersPerSecond, theta);
    result.travelTimes.armBSeconds = roundTripTimeSeconds(
        p.armLengthMeters, p.frameSpeedMetersPerSecond, armBTheta);

    result.timeDifferenceSeconds =
        result.travelTimes.armASeconds - result.travelTimes.armBSeconds;
    result.opticalPathDifferenceMeters =
        constants::SpeedOfLightMetersPerSecond * result.timeDifferenceSeconds;
    result.fringeOffset = result.opticalPathDifferenceMeters / p.wavelengthMeters;

    // Historical observable: rotate the apparatus by 90 degrees and compare
    // the phase/fringe offset before versus after rotation.
    const double thetaRotated = theta + 90.0 * constants::DegreesToRadians;
    const double armBThetaRotated = thetaRotated + 90.0 * constants::DegreesToRadians;

    const double rotatedDeltaT =
        roundTripTimeSeconds(p.armLengthMeters, p.frameSpeedMetersPerSecond, thetaRotated) -
        roundTripTimeSeconds(p.armLengthMeters, p.frameSpeedMetersPerSecond, armBThetaRotated);

    const double rotatedFringeOffset =
        constants::SpeedOfLightMetersPerSecond * rotatedDeltaT / p.wavelengthMeters;

    result.ninetyDegreeRotationFringeShift =
        rotatedFringeOffset - result.fringeOffset;

    return result;
}

} // namespace framelab
