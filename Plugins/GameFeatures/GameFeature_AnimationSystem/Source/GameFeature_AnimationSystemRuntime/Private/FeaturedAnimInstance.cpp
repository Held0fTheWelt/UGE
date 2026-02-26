// Copyright Epic Games, Inc. All Rights Reserved.

#include "FeaturedAnimInstance.h"
#include "Interfaces/CharacterGroundInfoProviderInterface.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(FeaturedAnimInstance)

UFeaturedAnimInstance::UFeaturedAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedAbilitySystemComponent = nullptr;
	GroundDistance = -1.0f;
}

void UFeaturedAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}

#if WITH_EDITOR
EDataValidationResult UFeaturedAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UFeaturedAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}

	CachedAbilitySystemComponent = GetAbilitySystemComponent();
}

void UFeaturedAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedAbilitySystemComponent)
	{
		CachedAbilitySystemComponent = GetAbilitySystemComponent();
	}

	// Ground distance via Elemental interface: prefer component (e.g. HumanoidMovementComponent), then owner (e.g. CoreCharacter)
	GroundDistance = -1.0f;
	if (AActor* Owner = GetOwningActor())
	{
		for (UActorComponent* Comp : Owner->GetComponents())
		{
			if (const ICharacterGroundInfoProviderInterface* GroundProvider = Cast<ICharacterGroundInfoProviderInterface>(Comp))
			{
				GroundDistance = GroundProvider->GetGroundDistance();
				break;
			}
		}
		if (GroundDistance < 0.0f)
		{
			if (const ICharacterGroundInfoProviderInterface* GroundProvider = Cast<ICharacterGroundInfoProviderInterface>(Owner))
			{
				GroundDistance = GroundProvider->GetGroundDistance();
			}
		}
	}
}

UAbilitySystemComponent* UFeaturedAnimInstance::GetAbilitySystemComponent() const
{
	if (AActor* Owner = GetOwningActor())
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	}
	return nullptr;
}

float UFeaturedAnimInstance::GetAttributeValue(FGameplayTag AttributeTag) const
{
	if (CachedAbilitySystemComponent)
	{
		// Get the attribute value from the ability system component
		// This is a simplified version - in a full implementation, you would query specific attribute sets
		FGameplayAttribute Attribute;
		// TODO: Implement attribute lookup based on tag
		// For now, return 0.0f as placeholder
		return 0.0f;
	}

	return 0.0f;
}

bool UFeaturedAnimInstance::IsAbilityActive(FGameplayTag AbilityTag) const
{
	if (CachedAbilitySystemComponent)
	{
		FGameplayTagContainer AbilityTags;
		CachedAbilitySystemComponent->GetOwnedGameplayTags(AbilityTags);
		return AbilityTags.HasTag(AbilityTag);
	}

	return false;
}

bool UFeaturedAnimInstance::HasGameplayTag(FGameplayTag TagToCheck) const
{
	if (CachedAbilitySystemComponent)
	{
		FGameplayTagContainer OwnedTags;
		CachedAbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);
		return OwnedTags.HasTag(TagToCheck);
	}

	return false;
}
