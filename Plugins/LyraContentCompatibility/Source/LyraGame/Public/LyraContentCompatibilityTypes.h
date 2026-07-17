#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Components/ActorComponent.h"
#include "Components/Widget.h"
#include "Components/GameFrameworkComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFeatureAction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "CharacterPartTypes.h"
#include "InputModifiers.h"
#include "InventoryItemDefinition.h"
#include "Abilities/GameplayAbility_FromEquipment.h"
#include "Equipment/EquipmentInstance.h"
#include "NumberPops/NumberPopComponent.h"
#include "UI/CoreActivatableWidget.h"
#include "LyraContentCompatibilityTypes.generated.h"

class APlayerState;
class UCommonButtonBase;
class UCommonUserWidget;
class UWidget;
class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraVerbMessage
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) FGameplayTag Verb;
    UPROPERTY(BlueprintReadWrite) TObjectPtr<UObject> Instigator = nullptr;
    UPROPERTY(BlueprintReadWrite) TObjectPtr<UObject> Target = nullptr;
    UPROPERTY(BlueprintReadWrite) FGameplayTagContainer InstigatorTags;
    UPROPERTY(BlueprintReadWrite) FGameplayTagContainer TargetTags;
    UPROPERTY(BlueprintReadWrite) FGameplayTagContainer ContextTags;
    UPROPERTY(BlueprintReadWrite) double Magnitude = 1.0;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraCharacterPart
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AActor> PartClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SocketName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECharacterCustomizationCollisionMode CollisionMode = ECharacterCustomizationCollisionMode::NoCollision;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraInteractionDurationMessage
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) TObjectPtr<AActor> Instigator = nullptr;
    UPROPERTY(BlueprintReadWrite) float Duration = 0.0f;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraTabDescriptor
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName TabId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText TabText;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FSlateBrush IconBrush;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bHidden = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TSubclassOf<UCommonButtonBase> TabButtonType;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TSubclassOf<UCommonUserWidget> TabContentType;
    UPROPERTY(Transient) TObjectPtr<UWidget> CreatedTabContentWidget = nullptr;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraAbilityMontageFailureMessage
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) TObjectPtr<APlayerController> PlayerController = nullptr;
    UPROPERTY(BlueprintReadWrite) FGameplayTagContainer FailureTags;
    UPROPERTY(BlueprintReadWrite) TObjectPtr<UAnimMontage> FailureMontage = nullptr;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraNotificationMessage
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category=Notification) FGameplayTag TargetChannel;
    UPROPERTY(BlueprintReadWrite, Category=Notification) TObjectPtr<APlayerState> TargetPlayer = nullptr;
    UPROPERTY(BlueprintReadWrite, Category=Notification) FText PayloadMessage;
    UPROPERTY(BlueprintReadWrite, Category=Notification) FGameplayTag PayloadTag;
    UPROPERTY(BlueprintReadWrite, Category=Notification) TObjectPtr<UObject> PayloadObject = nullptr;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraAbilitySimpleFailureMessage
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) TObjectPtr<APlayerController> PlayerController = nullptr;
    UPROPERTY(BlueprintReadWrite) FGameplayTagContainer FailureTags;
    UPROPERTY(BlueprintReadWrite) FText UserFacingReason;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraPlayerResetMessage
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) TObjectPtr<AActor> OwnerPlayerState = nullptr;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraQuickBarSlotsChangedMessage
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly, Category=Inventory) TObjectPtr<AActor> Owner = nullptr;
    UPROPERTY(BlueprintReadOnly, Category=Inventory) TArray<TObjectPtr<UInventoryItemInstance>> Slots;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FLyraQuickBarActiveIndexChangedMessage
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly, Category=Inventory) TObjectPtr<AActor> Owner = nullptr;
    UPROPERTY(BlueprintReadOnly, Category=Inventory) int32 ActiveIndex = 0;
};

/** UE 5.4 editor-only Niagara hierarchy metadata is retained as loadable compatibility data. */
UCLASS()
class LYRAGAME_API UUGELyraNiagaraHierarchyRoot : public UObject
{
    GENERATED_BODY()
};

/** UE 5.6 replacement target for the retired GameplayCameras Matinee shake class. */
UCLASS(BlueprintType, Blueprintable)
class LYRAGAME_API UUGELyraMatineeCameraShake : public UCameraShakeBase
{
    GENERATED_BODY()
};

/** UE 5.6 replacement target for the retired Matinee shake pattern import. */
UCLASS(EditInlineNew, BlueprintType)
class LYRAGAME_API UUGELyraMatineeCameraShakePattern : public UCameraShakePattern
{
    GENERATED_BODY()
};

/** UE 5.6 replacement target for the retired AIModule PawnActions component. */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class LYRAGAME_API UUGELyraPawnActionsComponent : public UActorComponent
{
    GENERATED_BODY()
};

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
