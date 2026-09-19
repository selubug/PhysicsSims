#pragma once

#include <vector>

#include "FrameLab/IMichelsonMorleyModel.h"

namespace framelab {

struct SweepPoint {
    double orientationDegrees = 0.0;
    MichelsonMorleyResult result;
};

class MichelsonMorleyExperiment {
public:
    [[nodiscard]] static std::vector<SweepPoint> sweep(
        const IMichelsonMorleyModel& model,
        MichelsonMorleyParameters parameters,
        double startDegrees,
        double endDegrees,
        double stepDegrees);

    [[nodiscard]] static ModelComparison compare(
        const IMichelsonMorleyModel& first,
        const IMichelsonMorleyModel& second,
        const MichelsonMorleyParameters& parameters);
};

} // namespace framelab
