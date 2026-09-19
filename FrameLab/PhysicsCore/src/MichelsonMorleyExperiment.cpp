#include "FrameLab/MichelsonMorleyExperiment.h"

#include <stdexcept>

namespace framelab {

std::vector<SweepPoint> MichelsonMorleyExperiment::sweep(
    const IMichelsonMorleyModel& model,
    MichelsonMorleyParameters parameters,
    const double startDegrees,
    const double endDegrees,
    const double stepDegrees) {

    if (stepDegrees <= 0.0) {
        throw std::invalid_argument("stepDegrees must be positive");
    }
    if (endDegrees < startDegrees) {
        throw std::invalid_argument("endDegrees must be >= startDegrees");
    }

    std::vector<SweepPoint> points;
    for (double angle = startDegrees; angle <= endDegrees + 1e-12; angle += stepDegrees) {
        parameters.orientationDegrees = angle;
        points.push_back({angle, model.simulate(parameters)});
    }
    return points;
}

ModelComparison MichelsonMorleyExperiment::compare(
    const IMichelsonMorleyModel& first,
    const IMichelsonMorleyModel& second,
    const MichelsonMorleyParameters& parameters) {

    return {first.simulate(parameters), second.simulate(parameters)};
}

} // namespace framelab
