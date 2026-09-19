#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "FrameLab/ClassicalEtherModel.h"
#include "FrameLab/MichelsonMorleyExperiment.h"
#include "FrameLab/PhysicsConstants.h"
#include "FrameLab/SpecialRelativityModel.h"

namespace {
void printResult(const framelab::MichelsonMorleyResult& r) {
    std::cout << "\n" << r.modelName << "\n"
              << "  arm A round trip:       " << r.travelTimes.armASeconds << " s\n"
              << "  arm B round trip:       " << r.travelTimes.armBSeconds << " s\n"
              << "  delta t:                " << r.timeDifferenceSeconds << " s\n"
              << "  optical path delta:     " << r.opticalPathDifferenceMeters << " m\n"
              << "  fringe offset:          " << r.fringeOffset << " fringes\n"
              << "  shift after 90 deg:     " << r.ninetyDegreeRotationFringeShift
              << " fringes\n";
}
}

int main() {
    using namespace framelab;

    MichelsonMorleyParameters parameters;
    parameters.armLengthMeters = 11.0;
    parameters.wavelengthMeters = constants::DefaultVisibleWavelengthMeters;
    parameters.frameSpeedMetersPerSecond = constants::EarthOrbitalSpeedMetersPerSecond;
    parameters.orientationDegrees = 0.0;

    ClassicalEtherModel ether;
    SpecialRelativityModel relativity;

    std::cout << std::scientific << std::setprecision(12);
    std::cout << "FrameLab - Michelson-Morley model comparison\n"
              << "L = " << parameters.armLengthMeters << " m\n"
              << "lambda = " << parameters.wavelengthMeters << " m\n"
              << "v = " << parameters.frameSpeedMetersPerSecond << " m/s\n"
              << "orientation = " << parameters.orientationDegrees << " deg\n";

    const ModelComparison comparison = MichelsonMorleyExperiment::compare(
        ether, relativity, parameters);

    printResult(comparison.first);
    printResult(comparison.second);

    // Generate data that can later drive an Unreal graph, material, or animation.
    std::ofstream csv("michelson_morley_sweep.csv");
    if (!csv) {
        std::cerr << "Could not create michelson_morley_sweep.csv\n";
        return 1;
    }

    csv << "angle_degrees,ether_fringe_offset,ether_rotation_shift,sr_fringe_offset\n";

    const auto etherSweep = MichelsonMorleyExperiment::sweep(
        ether, parameters, 0.0, 180.0, 1.0);
    const auto srSweep = MichelsonMorleyExperiment::sweep(
        relativity, parameters, 0.0, 180.0, 1.0);

    for (std::size_t i = 0; i < etherSweep.size(); ++i) {
        csv << etherSweep[i].orientationDegrees << ','
            << etherSweep[i].result.fringeOffset << ','
            << etherSweep[i].result.ninetyDegreeRotationFringeShift << ','
            << srSweep[i].result.fringeOffset << '\n';
    }

    std::cout << "\nWrote 181-point sweep to michelson_morley_sweep.csv\n";
    return 0;
}
