// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "FeaturedAnimInstance.generated.h"

class UAbilitySystemComponent;
struct FGameplayTag;

/**
 * Base animation instance used by this project.
 * Communicates with ability system and ground info only via engine/Elemental interfaces.
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEFEATURE_ANIMATIONSYSTEMRUNTIME_API UFeaturedAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFeaturedAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

	//~UAnimInstance interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~End of UAnimInstance interface

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

	/** Returns the ability system component from the pawn owner (via engine GAS). */
	UFUNCTION(BlueprintCallable, Category = "Animation")
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	/** Returns the current value of an attribute from the ability system. */
	UFUNCTION(BlueprintCallable, Category = "Animation", Meta = (GameplayTagFilter = "Attribute"))
	float GetAttributeValue(FGameplayTag AttributeTag) const;

	/** Returns true if the specified ability is active. */
	UFUNCTION(BlueprintCallable, Category = "Animation", Meta = (GameplayTagFilter = "Ability"))
	bool IsAbilityActive(FGameplayTag AbilityTag) const;

	/** Returns true if the specified tag is present. */
	UFUNCTION(BlueprintCallable, Category = "Animation")
	bool HasGameplayTag(FGameplayTag TagToCheck) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAbilitySystemComponent> CachedAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
