#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Camera/CameraComponent.h"
#include "Components/Widget.h"
#include "Components/GameFrameworkComponent.h"
#include "GameFeatureAction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputModifiers.h"
#include "InventoryItemDefinition.h"
#include "Abilities/GameplayAbility_FromEquipment.h"
#include "Equipment/EquipmentInstance.h"
#include "NumberPops/NumberPopComponent.h"
#include "UI/CoreActivatableWidget.h"
#include "LyraContentCompatibilityTypes.generated.h"

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraContextEffectAnimNotifyAudioSettings
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float VolumeMultiplier = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float PitchMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraContextEffectAnimNotifyTraceSettings
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector TraceStartOffset = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector TraceEndOffset = FVector(0.0, 0.0, -50.0);
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraContextEffectAnimNotifyVFXSettings
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Scale = FVector::OneVector;
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API UAnimNotify_LyraContextEffects : public UAnimNotify
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API UApplyFrontendPerfSettingsAction : public UGameFeatureAction
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API UCircumferenceMarkerWidget : public UWidget
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API UHitMarkerConfirmationWidget : public UWidget
{
    GENERATED_BODY()
};

UCLASS(DefaultToInstanced, EditInlineNew)
class LYRAGAME_API UInventoryFragment_ReticleConfig : public UInventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<TSubclassOf<UUserWidget>> ReticleWidgetClasses;
};

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Camera), meta=(BlueprintSpawnableComponent))
class LYRAGAME_API ULyraCameraComponent : public UCameraComponent
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API ULyraCameraMode_ThirdPerson : public UObject
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API ULyraCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Health), meta=(BlueprintSpawnableComponent))
class LYRAGAME_API ULyraHealthComponent : public UGameFrameworkComponent
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, EditInlineNew)
class LYRAGAME_API ULyraSettingBasedScalar : public UInputModifier
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, EditInlineNew)
class LYRAGAME_API ULyraInputModifierAimInversion : public UInputModifier
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, EditInlineNew)
class LYRAGAME_API ULyraInputModifierDeadZone : public UInputModifier
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, EditInlineNew)
class LYRAGAME_API ULyraInputModifierGamepadSensitivity : public UInputModifier
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API ULyraNumberPopComponent_NiagaraText : public UNumberPopComponent
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API ALyraPlayerBotController : public AAIController
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API ULyraRangedWeaponInstance : public UEquipmentInstance
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API ULyraGameplayAbility_RangedWeapon : public UGameplayAbility_FromEquipment
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API ULyraSettingScreen : public UCoreActivatableWidget
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API ALyraWeaponSpawner : public AActor
{
    GENERATED_BODY()
};
