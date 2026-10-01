#include "FrameLabInterferometerActor.h"

#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

AFrameLabInterferometerActor::AFrameLabInterferometerActor()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    Simulation = CreateDefaultSubobject<UMichelsonMorleyComponent>(TEXT("Simulation"));
}

void AFrameLabInterferometerActor::BeginPlay()
{
    Super::BeginPlay();
    RefreshSimulation();
}

void AFrameLabInterferometerActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (bAutoRotate && FMath::IsFinite(RotationDegreesPerSecond))
        OrientationDegrees = FMath::Fmod(
            OrientationDegrees + RotationDegreesPerSecond * DeltaSeconds, 360.0);
    RefreshSimulation();
    if (bDrawPreview)
        DrawPreview();
}

bool AFrameLabInterferometerActor::RefreshSimulation()
{
    bResultsValid = Simulation->CompareModels(ArmLengthMeters, WavelengthNanometers,
        FrameSpeedMetersPerSecond, OrientationDegrees, EtherResult, RelativityResult, LastError);
    OnSimulationUpdated(bResultsValid);
    return bResultsValid;
}

void AFrameLabInterferometerActor::DrawPreview() const
{
    UWorld* World = GetWorld();
    if (!World)
        return;
    const FVector Center = GetActorLocation();
    if (!bResultsValid)
    {
        DrawDebugString(World, Center + FVector(0, 0, 60), LastError,
            nullptr, FColor::Red, 0.0f, true);
        return;
    }
    if (!FMath::IsFinite(DisplayScale) || DisplayScale <= 0.0)
        return;
    // Meters -> Unreal centimeters, then apply display magnification only here.
    const double LengthCm = ArmLengthMeters * 100.0 * DisplayScale;
    if (!FMath::IsFinite(LengthCm) || LengthCm > 1e7)
        return;
    const double Angle = FMath::DegreesToRadians(FMath::Fmod(OrientationDegrees, 360.0));
    const FTransform Transform = GetActorTransform();
    const FVector ArmA = Transform.TransformPosition(
        FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0) * LengthCm);
    const FVector ArmB = Transform.TransformPosition(
        FVector(-FMath::Sin(Angle), FMath::Cos(Angle), 0) * LengthCm);
    DrawDebugLine(World, Center, ArmA, FColor::Cyan, false, 0.0f, 0, 3.0f);
    DrawDebugLine(World, Center, ArmB, FColor::Yellow, false, 0.0f, 0, 3.0f);
    DrawDebugSphere(World, Center, 8.0f, 12, FColor::White, false, 0.0f);
    DrawDebugSphere(World, ArmA, 8.0f, 12, FColor::Cyan, false, 0.0f);
    DrawDebugSphere(World, ArmB, 8.0f, 12, FColor::Yellow, false, 0.0f);
    // Ether-wind reference remains fixed relative to the actor while the arms rotate.
    const FVector WindEnd = Center + Transform.TransformVectorNoScale(FVector(100, 0, 0));
    DrawDebugDirectionalArrow(World, Center, WindEnd, 15.0f, FColor::Green, false, 0.0f);
    const FString Label = FString::Printf(
        TEXT("Angle %.1f deg | L %.2f m | wavelength %.1f nm\n")
        TEXT("Ether: offset %.6f | 90-deg shift %.6f fringes\n")
        TEXT("SR: offset %.6f | 90-deg shift %.6f fringes\n")
        TEXT("A/B round trips (ether): %.4f / %.4f ns"),
        OrientationDegrees, ArmLengthMeters, WavelengthNanometers,
        EtherResult.FringeOffset, EtherResult.RotationFringeShift,
        RelativityResult.FringeOffset, RelativityResult.RotationFringeShift,
        EtherResult.ArmATimeSeconds * 1e9, EtherResult.ArmBTimeSeconds * 1e9);
    DrawDebugString(World, Center + FVector(0, 0, 60), Label,
        nullptr, FColor::White, 0.0f, true);
}
