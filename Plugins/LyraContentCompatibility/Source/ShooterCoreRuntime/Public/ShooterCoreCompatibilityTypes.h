#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "InputModifiers.h"
#include "GameplayTagContainer.h"
#include "ShooterCoreCompatibilityTypes.generated.h"


UCLASS(BlueprintType, EditInlineNew)
class SHOOTERCORERUNTIME_API UAimAssistInputModifier : public UInputModifier
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable, ClassGroup=(AimAssist), meta=(BlueprintSpawnableComponent))
class SHOOTERCORERUNTIME_API UAimAssistTargetComponent : public UCapsuleComponent
{
    GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable, ClassGroup=(AimAssist), meta=(BlueprintSpawnableComponent))
class SHOOTERCORERUNTIME_API UAimAssistTargetManagerComponent : public UActorComponent
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct SHOOTERCORERUNTIME_API FLyraControlPointStatusMessage
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGameplayTag ControlPointTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TeamId = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct SHOOTERCORERUNTIME_API FLyraAccoladeDefinitionRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TSoftObjectPtr<USoundBase> Sound;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TSoftObjectPtr<UObject> Icon;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float DisplayDuration = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTag LocationTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTagContainer AccoladeTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTagContainer CancelAccoladesWithTag;
};
