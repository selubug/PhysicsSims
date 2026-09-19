# Unreal integration plan

`PhysicsCore` intentionally contains no Unreal headers or Unreal types. Keep it that way.

Tomorrow, FrameLab can add a runtime Unreal module that owns a thin adapter around the portable library:

```text
FrameLab Unreal project
Source/
  FrameLab/                    <- normal game / visualization code
  FrameLabPhysics/             <- runtime module / bridge
    Public/
    Private/
```

Epic's module conventions use a module `Build.cs` file plus `Public` and `Private` directories. The bridge should convert Unreal-facing values to the portable SI-unit structures, call `framelab::ClassicalEtherModel` or `framelab::SpecialRelativityModel`, and convert only the *visual position data* to Unreal centimeters.

Suggested Unreal-facing component:

```cpp
UENUM(BlueprintType)
enum class EFrameLabLightModel : uint8 {
    ClassicalEther,
    SpecialRelativity
};

USTRUCT(BlueprintType)
struct FFrameLabInterferometerResult {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    double ArmATimeSeconds = 0.0;

    UPROPERTY(BlueprintReadOnly)
    double ArmBTimeSeconds = 0.0;

    UPROPERTY(BlueprintReadOnly)
    double FringeOffset = 0.0;

    UPROPERTY(BlueprintReadOnly)
    double RotationFringeShift = 0.0;
};

UCLASS(ClassGroup=(FrameLab), meta=(BlueprintSpawnableComponent))
class UMichelsonMorleyComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FFrameLabInterferometerResult Simulate(
        EFrameLabLightModel Model,
        double ArmLengthMeters,
        double WavelengthNanometers,
        double FrameSpeedMetersPerSecond,
        double OrientationDegrees) const;
};
```

This lets the Unreal level, UMG sliders, Blueprint animation, beam material, and C++ Actors call the *same tested equations* as the CLI.

## Visualization rule

Keep two concepts separate:

1. **Physical result** — returned by PhysicsCore, always SI units and mathematically faithful.
2. **Display state** — Unreal animation speed, beam trail length, visual exaggeration, world scale, and camera behavior.

Never change `c`, `v`, or the model equations just to make an effect visible. Exaggerate only the presentation.
