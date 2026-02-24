// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/CoreGameplayAbility.h"
#include "InteractionOption.generated.h"

template <typename InterfaceType> class TScriptInterface;
class IInteractableTarget;
class UUserWidget;
class UAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayAbilitySpecHandle;

/**
 * FInteractionOption
 *
 *	Represents a single interaction option available to the player.
 *	Adapted from FLyraInteractionOption.
 */
USTRUCT(BlueprintType)
struct CORE_INTERACT_API FInteractionOption
{
	GENERATED_BODY()

public:
	FInteractionOption()
		: InteractableTarget(nullptr)
		, Text()
		, SubText()
		, InteractionAbilityToGrant(nullptr)
		, TargetAbilitySystem(nullptr)
		, TargetInteractionAbilityHandle()
		, InteractionWidgetClass()
	{
	}

	// The interactable target
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IInteractableTarget> InteractableTarget;

	// Simple text the interaction might return
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	// Simple sub-text the interaction might return
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SubText;

	// The ability to grant the avatar when they get near interactable objects.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> InteractionAbilityToGrant;

	// The ability system on the target that can be used for the TargetInteractionHandle and sending the event, if needed.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystem = nullptr;

	// The ability spec to activate on the object for this option.
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle TargetInteractionAbilityHandle;

	// The widget to show for this kind of interaction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> InteractionWidgetClass;

public:
	FORCEINLINE bool operator==(const FInteractionOption& Other) const
	{
		return InteractableTarget == Other.InteractableTarget &&
			InteractionAbilityToGrant == Other.InteractionAbilityToGrant &&
			TargetAbilitySystem == Other.TargetAbilitySystem &&
			TargetInteractionAbilityHandle == Other.TargetInteractionAbilityHandle &&
			InteractionWidgetClass == Other.InteractionWidgetClass &&
			Text.IdenticalTo(Other.Text) &&
			SubText.IdenticalTo(Other.SubText);
	}

	FORCEINLINE bool operator!=(const FInteractionOption& Other) const
	{
		return !operator==(Other);
	}

	FORCEINLINE bool operator<(const FInteractionOption& Other) const
	{
		return InteractableTarget.GetInterface() < Other.InteractableTarget.GetInterface();
	}
};
