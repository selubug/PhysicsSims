#include "MichelsonMorleyComponent.h"

#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "FrameLab/ClassicalEtherModel.h"
#include "FrameLab/InterferometerInput.h"
#include <limits>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFrameLabBridgeTest, "FrameLab.Bridge.ModelComparison",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FFrameLabBridgeTest::RunTest(const FString& Parameters)
{
    UMichelsonMorleyComponent* Component = NewObject<UMichelsonMorleyComponent>();
    FFrameLabInterferometerResult Ether, Relativity;
    FString Error;
    TestTrue(TEXT("Default comparison succeeds"),
        Component->CompareModels(11, 500, 29780, 0, Ether, Relativity, Error));
    const auto Expected = framelab::ClassicalEtherModel{}.simulate(
        framelab::parametersFromDisplayUnits(11, 500, 29780, 0));
    TestEqual(TEXT("Adapter uses the same core result"), Ether.FringeOffset, Expected.fringeOffset);
    TestEqual(TEXT("Arm A mapped in seconds"), Ether.ArmATimeSeconds, Expected.travelTimes.armASeconds);
    TestEqual(TEXT("Arm B mapped in seconds"), Ether.ArmBTimeSeconds, Expected.travelTimes.armBSeconds);
    TestEqual(TEXT("Time difference mapped"), Ether.TimeDifferenceSeconds, Expected.timeDifferenceSeconds);
    TestEqual(TEXT("Optical path mapped in meters"), Ether.OpticalPathDifferenceMeters, Expected.opticalPathDifferenceMeters);
    TestEqual(TEXT("Signed rotation shift mapped"), Ether.RotationFringeShift, Expected.ninetyDegreeRotationFringeShift);
    TestEqual(TEXT("SR offset is zero"), Relativity.FringeOffset, 0.0);
    TestTrue(TEXT("Historical shift magnitude"), FMath::Abs(Ether.RotationFringeShift) > 0.35 &&
        FMath::Abs(Ether.RotationFringeShift) < 0.50);
    TestTrue(TEXT("45 degree simulation succeeds"),
        Component->Simulate(EFrameLabLightModel::ClassicalEther, 11, 500, 29780, 45, Ether, Error));
    TestTrue(TEXT("45 degree offset vanishes"), FMath::Abs(Ether.FringeOffset) < 1e-7);
    TestFalse(TEXT("NaN rejected"), Component->CompareModels(11, 500, 29780,
        std::numeric_limits<double>::quiet_NaN(), Ether, Relativity, Error));
    TestTrue(TEXT("Failure explains why"), !Error.IsEmpty());
    TestTrue(TEXT("Failure clears both results"), Ether.ModelName.IsEmpty() && Relativity.ModelName.IsEmpty());
    TestFalse(TEXT("Speed at c rejected"), Component->Simulate(
        EFrameLabLightModel::SpecialRelativity, 11, 500, 299792458, 0, Ether, Error));
    TestFalse(TEXT("Unknown model rejected"), Component->Simulate(
        static_cast<EFrameLabLightModel>(255), 11, 500, 29780, 0, Ether, Error));
    TestFalse(TEXT("Overflowing prediction rejected"), Component->Simulate(
        EFrameLabLightModel::ClassicalEther, std::numeric_limits<double>::max(),
        500, 29780, 0, Ether, Error));
    TestTrue(TEXT("Valid request recovers after failures"),
        Component->CompareModels(11, 500, 0, 360, Ether, Relativity, Error));
    TestTrue(TEXT("Success clears the previous error"), Error.IsEmpty());
    TestTrue(TEXT("Models agree at rest"),
        FMath::Abs(Ether.ArmATimeSeconds - Relativity.ArmATimeSeconds) < 1e-20);
    return true;
}
#endif
