#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MichelsonMorleyComponent.h"
#include "FrameLabInterferometerActor.generated.h"

// Development preview. Subclass in Blueprint to add meshes, materials, and UMG.
UCLASS(Blueprintable)
class FRAMELABPHYSICS_API AFrameLabInterferometerActor : public AActor
{
    GENERATED_BODY()

public:
    AFrameLabInterferometerActor();
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FrameLab")
    TObjectPtr<UMichelsonMorleyComponent> Simulation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameLab|Physics", meta=(ClampMin="0.000001", Units="m"))
    double ArmLengthMeters = 11.0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameLab|Physics", meta=(ClampMin="0.000001"))
    double WavelengthNanometers = 500.0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameLab|Physics", meta=(ClampMin="0.0", ClampMax="299792457.0"))
    double FrameSpeedMetersPerSecond = 29780.0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameLab|Physics")
    double OrientationDegrees = 0.0;

    // Only preview geometry uses these values. Actor scale is also display-only.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameLab|Display", meta=(ClampMin="0.001"))
    double DisplayScale = 0.25;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameLab|Display")
    bool bDrawPreview = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameLab|Display")
    bool bAutoRotate = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameLab|Display")
    double RotationDegreesPerSecond = 15.0;

    UPROPERTY(BlueprintReadOnly, Category="FrameLab") FFrameLabInterferometerResult EtherResult;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") FFrameLabInterferometerResult RelativityResult;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") bool bResultsValid = false;
    UPROPERTY(BlueprintReadOnly, Category="FrameLab") FString LastError;

    UFUNCTION(BlueprintCallable, CallInEditor, Category="FrameLab")
    bool RefreshSimulation();

    // Bind UMG or material parameters here; results remain unscaled.
    UFUNCTION(BlueprintImplementableEvent, Category="FrameLab")
    void OnSimulationUpdated(bool bSuccess);

protected:
    virtual void BeginPlay() override;

private:
    void DrawPreview() const;
};
