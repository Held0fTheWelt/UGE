#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "HeliFlightPresetDataAsset.generated.h"

UENUM(BlueprintType)
enum class EHeliFlightMode : uint8
{
    Realistic    UMETA(DisplayName="Realistic"),
    SAS_Assist   UMETA(DisplayName="SAS Assist"),
    Arcade       UMETA(DisplayName="Arcade"),
};

UENUM(BlueprintType)
enum class EHeliSASHoldMode : uint8
{
    Off         UMETA(DisplayName="Off"),
    LevelHold   UMETA(DisplayName="Level Hold"),
    TrimHold    UMETA(DisplayName="Trim Hold"),
};

USTRUCT(BlueprintType)
struct FHeliDiscTiltConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DiscTilt")
    float MaxTiltDeg = 14.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DiscTilt")
    float TiltRateDegPerSec = 60.0f;
};

USTRUCT(BlueprintType)
struct FHeliDragConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drag")
    float LinearCoeff = 0.09f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drag")
    float QuadraticCoeff = 0.0f;
};

USTRUCT(BlueprintType)
struct FHeliDampingConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damping")
    float LinearDamping = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damping")
    float AngularDamping = 2.0f;

    // Only really used in Arcade (optional)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damping")
    float ExtraLinearDamping = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damping")
    float ExtraAngularDamping = 0.0f;
};

USTRUCT(BlueprintType)
struct FHeliSASConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS")
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS")
    bool bEnableRateDamping = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS")
    bool bYawDamperEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS")
    EHeliSASHoldMode HoldMode = EHeliSASHoldMode::LevelHold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Deadzones")
    float CyclicDeadzone = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Deadzones")
    float PedalsDeadzone = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Blend")
    float HoldEngageDelaySec = 0.10f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Blend")
    float HoldBlendInSec = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Blend")
    float HoldBlendOutSec = 0.10f;

    // Body-local gains (Roll, Pitch, Yaw)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Gains")
    FVector RateDampingGain = FVector(2400.f, 2400.f, 1600.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Gains")
    FVector AttitudeHoldGain = FVector(2000.f, 2000.f, 0.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Gains")
    FVector HoldDampingGain = FVector(750.f, 750.f, 0.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Limits")
    FVector MaxAssistTorqueNm = FVector(8000.f, 8000.f, 3500.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Yaw")
    bool bYawAngleHoldInTrim = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Yaw", meta=(EditCondition="bYawAngleHoldInTrim"))
    float YawHoldGain = 0.0f;
};

USTRUCT(BlueprintType)
struct FHeliArcadeConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade")
    bool bEnableHelperForces = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade")
    float AssistStrength = 1.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade|Helpers")
    float ForwardForceScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade|Helpers")
    float SideForceScale = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade|Helpers")
    float HelperTorqueScale = 0.6f;
};

/** Optional cyclic torque gain overrides for this flight mode (if not using helicopter-default values) */
USTRUCT(BlueprintType)
struct FHeliCyclicConfig
{
    GENERATED_BODY()

    /** Override cyclic pitch/roll torque gains for this mode (e.g. Arcade might want higher/lower). If false, use helicopter default. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cyclic")
    bool bOverrideCyclicGains = false;

    /** Explicit pitch torque from cyclic disc tilt (Nm per degree of tilt). Only used if bOverrideCyclicGains=true. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cyclic", meta=(EditCondition="bOverrideCyclicGains"))
    float CyclicPitchTorqueGain = 50.f;

    /** Explicit roll torque from cyclic disc tilt (Nm per degree of tilt). Only used if bOverrideCyclicGains=true. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cyclic", meta=(EditCondition="bOverrideCyclicGains"))
    float CyclicRollTorqueGain = 50.f;

    /** Minimum thrust (N) when disc is tilted via cyclic. Allows cyclic without collective. 0 = disabled. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cyclic", meta=(EditCondition="bOverrideCyclicGains"))
    float CyclicIdleThrust = 0.f;
};

/** Main rotor reactive torque configuration. The main rotor spinning produces a reaction torque
    on the fuselage (Newton's 3rd law). The tail rotor must compensate. */
USTRUCT(BlueprintType)
struct FHeliMainRotorTorqueConfig
{
    GENERATED_BODY()

    /** Enable reactive torque from main rotor on the fuselage. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainRotorTorque")
    bool bEnabled = true;

    /** Scale factor for the reactive torque. Higher = stronger yaw tendency.
        ReactiveYaw = MainRotorTorqueScale * Collective * NormalizedRPM² * RotorDirection */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainRotorTorque", meta=(ClampMin="0"))
    float MainRotorTorqueScale = 8.0f;

    /** Main rotor spin direction: +1 = CCW from above (US/EU convention, torque pushes nose right),
        -1 = CW from above (Russian convention, torque pushes nose left). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainRotorTorque", meta=(ClampMin="-1", ClampMax="1"))
    float RotorDirectionSign = 1.0f;

    /** Lateral force applied at tail rotor position (N per unit pedal input × rotor speed × torque reduction).
        Produces realistic side drift. 0 = pure yaw torque only (legacy). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TailRotor", meta=(ClampMin="0"))
    float TailRotorForceScale = 0.f;
};

/** P0.5: Optional center-of-mass offset (local space) for tuning; applied to physics body when enabled. */
USTRUCT(BlueprintType)
struct FHeliCOMConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COM")
    bool bEnableOverride = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COM", meta=(EditCondition="bEnableOverride"))
    FVector CenterOfMassOffset = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FHeliModePreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    EHeliFlightMode Mode = EHeliFlightMode::SAS_Assist;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FHeliDiscTiltConfig DiscTilt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FHeliDragConfig Drag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FHeliDampingConfig Damping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FHeliSASConfig SAS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FHeliArcadeConfig Arcade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FHeliCyclicConfig Cyclic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset|COM")
    FHeliCOMConfig COM;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset|MainRotorTorque")
    FHeliMainRotorTorqueConfig MainRotorTorque;
};

UCLASS(BlueprintType)
class HELICOPTERS_API UHeliFlightPresetDataAsset : public UPreLoadingDataAsset
{
    GENERATED_BODY()

public:
    UHeliFlightPresetDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
        : Super(ObjectInitializer)
    {
        AssetTypeName = GetAssetType();
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Preset")
    FHeliModePreset Preset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Meta")
    int32 Version = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Meta")
    FString Notes;

    virtual const FPrimaryAssetType GetAssetType_Implementation() const override
    {
        return AssetType;
    }

    static FPrimaryAssetType GetAssetType()
    {
        return AssetType;
    }

private:
    static const FPrimaryAssetType AssetType;
};
