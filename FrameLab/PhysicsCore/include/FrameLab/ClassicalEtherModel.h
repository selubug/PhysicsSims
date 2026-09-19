#pragma once

#include "FrameLab/IMichelsonMorleyModel.h"

namespace framelab {

// Pre-relativistic stationary-ether model.
// Light propagates at c in the ether frame while the apparatus moves at v.
class ClassicalEtherModel final : public IMichelsonMorleyModel {
public:
    [[nodiscard]] MichelsonMorleyResult simulate(
        const MichelsonMorleyParameters& parameters) const override;

private:
    [[nodiscard]] static double roundTripTimeSeconds(
        double armLengthMeters,
        double frameSpeedMetersPerSecond,
        double armAngleRadians);
};

} // namespace framelab
