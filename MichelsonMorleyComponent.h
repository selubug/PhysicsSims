#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MichelsonMorleyComponent.generated.h"

UENUM(BlueprintType)
enum class EFrameLabLightModel : uint8
{
    ClassicalEther,
    SpecialRelativity
};

USTRUCT(BlueprintType)
struct FRAMELABPHYSICS_API FFrameLabInterferometerResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="FrameLab") FString ModelName;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") double ArmATimeSeconds = 0.0;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") double ArmBTimeSeconds = 0.0;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") double TimeDifferenceSeconds = 0.0;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") double OpticalPathDifferenceMeters = 0.0;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") double FringeOffset = 0.0;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") double RotationFringeShift = 0.0;
};

UCLASS(ClassGroup=(FrameLab), meta=(BlueprintSpawnableComponent))
class FRAMELABPHYSICS_API UMichelsonMorleyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // False clears Result and explains the failure in Error; branch on the return value.
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="FrameLab")
    bool Simulate(EFrameLabLightModel Model, double ArmLengthMeters,
        double WavelengthNanometers, double FrameSpeedMetersPerSecond,
        double OrientationDegrees, FFrameLabInterferometerResult& Result, FString& Error) const;

    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="FrameLab")
    bool CompareModels(double ArmLengthMeters, double WavelengthNanometers,
        double FrameSpeedMetersPerSecond, double OrientationDegrees,
        FFrameLabInterferometerResult& EtherResult,
        FFrameLabInterferometerResult& RelativityResult, FString& Error) const;
};
