#include "MichelsonMorleyComponent.h"

#include <cmath>
#include <exception>
#include "FrameLab/ClassicalEtherModel.h"
#include "FrameLab/InterferometerInput.h"
#include "FrameLab/SpecialRelativityModel.h"

bool UMichelsonMorleyComponent::Simulate(EFrameLabLightModel Model,
    double ArmLengthMeters, double WavelengthNanometers,
    double FrameSpeedMetersPerSecond, double OrientationDegrees,
    FFrameLabInterferometerResult& Result, FString& Error) const
{
    Result = FFrameLabInterferometerResult{};
    Error.Reset();
    try
    {
        const auto Parameters = framelab::parametersFromDisplayUnits(ArmLengthMeters,
            WavelengthNanometers, FrameSpeedMetersPerSecond, OrientationDegrees);
        framelab::MichelsonMorleyResult Core;
        switch (Model)
        {
        case EFrameLabLightModel::ClassicalEther:
            Core = framelab::ClassicalEtherModel{}.simulate(Parameters);
            break;
        case EFrameLabLightModel::SpecialRelativity:
            Core = framelab::SpecialRelativityModel{}.simulate(Parameters);
            break;
        default:
            Error = TEXT("Unknown light model.");
            return false;
        }
        if (!std::isfinite(Core.travelTimes.armASeconds) ||
            !std::isfinite(Core.travelTimes.armBSeconds) ||
            !std::isfinite(Core.timeDifferenceSeconds) ||
            !std::isfinite(Core.opticalPathDifferenceMeters) ||
            !std::isfinite(Core.fringeOffset) ||
            !std::isfinite(Core.ninetyDegreeRotationFringeShift))
        {
            Error = TEXT("Inputs exceed the numerical range of this model.");
            return false;
        }
        Result.ModelName = UTF8_TO_TCHAR(Core.modelName.c_str());
        Result.ArmATimeSeconds = Core.travelTimes.armASeconds;
        Result.ArmBTimeSeconds = Core.travelTimes.armBSeconds;
        Result.TimeDifferenceSeconds = Core.timeDifferenceSeconds;
        Result.OpticalPathDifferenceMeters = Core.opticalPathDifferenceMeters;
        Result.FringeOffset = Core.fringeOffset;
        Result.RotationFringeShift = Core.ninetyDegreeRotationFringeShift;
        return true;
    }
    catch (const std::exception& Exception)
    {
        Error = UTF8_TO_TCHAR(Exception.what());
        return false;
    }
}

bool UMichelsonMorleyComponent::CompareModels(double ArmLengthMeters,
    double WavelengthNanometers, double FrameSpeedMetersPerSecond,
    double OrientationDegrees, FFrameLabInterferometerResult& EtherResult,
    FFrameLabInterferometerResult& RelativityResult, FString& Error) const
{
    // Use temporaries so a failed comparison never exposes a partial/stale result.
    EtherResult = FFrameLabInterferometerResult{};
    RelativityResult = FFrameLabInterferometerResult{};
    FFrameLabInterferometerResult Ether, Relativity;
    if (!Simulate(EFrameLabLightModel::ClassicalEther, ArmLengthMeters,
        WavelengthNanometers, FrameSpeedMetersPerSecond, OrientationDegrees, Ether, Error) ||
        !Simulate(EFrameLabLightModel::SpecialRelativity, ArmLengthMeters,
        WavelengthNanometers, FrameSpeedMetersPerSecond, OrientationDegrees, Relativity, Error))
        return false;
    EtherResult = Ether;
    RelativityResult = Relativity;
    return true;
}
