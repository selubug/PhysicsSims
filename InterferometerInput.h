#pragma once

#include <cmath>
#include <stdexcept>

#include "FrameLab/MichelsonMorleyTypes.h"
#include "FrameLab/PhysicsConstants.h"

namespace framelab {

// Shared UI boundary: wavelengths arrive in nanometers; models use SI units.
inline MichelsonMorleyParameters parametersFromDisplayUnits(
    double armLengthMeters, double wavelengthNanometers,
    double frameSpeedMetersPerSecond, double orientationDegrees) {
    if (!std::isfinite(armLengthMeters) || armLengthMeters <= 0.0 ||
        !std::isfinite(wavelengthNanometers) || wavelengthNanometers <= 0.0 ||
        !std::isfinite(frameSpeedMetersPerSecond) || frameSpeedMetersPerSecond < 0.0 ||
        frameSpeedMetersPerSecond >= constants::SpeedOfLightMetersPerSecond ||
        !std::isfinite(orientationDegrees)) {
        throw std::invalid_argument(
            "Use finite values: arm length and wavelength > 0, and 0 <= speed < c.");
    }
    const double wavelengthMeters = wavelengthNanometers * 1e-9;
    if (wavelengthMeters <= 0.0) {
        throw std::invalid_argument("Wavelength is too small to represent in meters.");
    }
    return {armLengthMeters, wavelengthMeters, frameSpeedMetersPerSecond,
            std::fmod(orientationDegrees, 360.0)};
}

} // namespace framelab
