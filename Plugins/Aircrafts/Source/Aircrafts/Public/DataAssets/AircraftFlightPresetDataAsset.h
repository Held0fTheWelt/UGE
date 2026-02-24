#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "AircraftFlightPresetDataAsset.generated.h"

UENUM(BlueprintType)
enum class EAircraftFlightMode : uint8
{
    Realistic    UMETA(DisplayName="Realistic"),
    SAS_Assist   UMETA(DisplayName="SAS Assist"),
    Arcade       UMETA(DisplayName="Arcade"),
};

// ── Wing aerodynamics ───────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAircraftWingConfig
{
    GENERATED_BODY()

    /** Reference wing area (m^2). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wing")
    float WingArea = 30.f;

    /** dCL/dAlpha (per radian). Typical 5.7 for finite wings. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wing")
    float LiftCurveSlope = 5.7f;

    /** Maximum lift coefficient (clean wing). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wing")
    float MaxLiftCoeff = 1.6f;

    /** Angle of attack (deg) at which CL peaks before stall. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wing")
    float StallAngleDeg = 15.f;
};

// ── Drag ────────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAircraftDragConfig
{
    GENERATED_BODY()

    /** Parasite (zero-lift) drag coefficient CD0. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drag")
    float ParasiteDragCoeff = 0.025f;

    /** Induced drag factor k (CD_induced = k * CL^2). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drag")
    float InducedDragFactor = 0.04f;
};

// ── Damping ─────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAircraftDampingConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damping")
    float LinearDamping = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damping")
    float AngularDamping = 2.f;
};

// ── Control surfaces ────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAircraftControlSurfaceConfig
{
    GENERATED_BODY()

    /** Maximum elevator deflection (deg). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ControlSurfaces")
    float ElevatorMaxDeg = 25.f;

    /** Maximum aileron deflection (deg). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ControlSurfaces")
    float AileronMaxDeg = 20.f;

    /** Maximum rudder deflection (deg). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ControlSurfaces")
    float RudderMaxDeg = 30.f;

    /** Rate at which control surfaces deflect (deg/s). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ControlSurfaces")
    float SurfaceDeflectionRate = 90.f;
};

// ── SAS (Stability Augmentation System) ─────────────────────────────

USTRUCT(BlueprintType)
struct FAircraftSASConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS")
    bool bEnabled = true;

    /** Body-local rate damping gains (Roll, Pitch, Yaw). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Gains")
    FVector RateDampingGain = FVector(2400.f, 2400.f, 1600.f);

    /** Attitude-hold gains (Roll, Pitch, Yaw). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Gains")
    FVector AttitudeHoldGain = FVector(2000.f, 2000.f, 0.f);

    /** Maximum SAS torque magnitude per axis (N*m). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Limits")
    FVector MaxAssistTorqueNm = FVector(8000.f, 8000.f, 3500.f);
};

// ── Arcade mode helpers ─────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAircraftArcadeConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade")
    bool bEnableHelperForces = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade")
    float AssistStrength = 1.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade")
    float LiftAssistScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arcade")
    float TorqueAssistScale = 0.6f;
};

// ── Center-of-Mass override ─────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAircraftCOMConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COM")
    bool bEnableOverride = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COM", meta=(EditCondition="bEnableOverride"))
    FVector CenterOfMassOffset = FVector::ZeroVector;
};

// ── Combined mode preset ────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAircraftModePreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    EAircraftFlightMode Mode = EAircraftFlightMode::SAS_Assist;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FAircraftWingConfig Wing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FAircraftDragConfig Drag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FAircraftDampingConfig Damping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FAircraftControlSurfaceConfig ControlSurfaces;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FAircraftSASConfig SAS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
    FAircraftArcadeConfig Arcade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset|COM")
    FAircraftCOMConfig COM;
};

// ── Data Asset ──────────────────────────────────────────────────────

UCLASS(BlueprintType)
class AIRCRAFTS_API UAircraftFlightPresetDataAsset : public UPreLoadingDataAsset
{
    GENERATED_BODY()

public:
    UAircraftFlightPresetDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
        : Super(ObjectInitializer)
    {
        AssetTypeName = GetAssetType();
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Preset")
    FAircraftModePreset Preset;

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
