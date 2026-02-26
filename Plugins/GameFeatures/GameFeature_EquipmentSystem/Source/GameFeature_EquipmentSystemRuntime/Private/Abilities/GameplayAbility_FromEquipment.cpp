// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Abilities/GameplayAbility_FromEquipment.h"
#include "Equipment/EquipmentInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_FromEquipment)

UGameplayAbility_FromEquipment::UGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UEquipmentInstance* UGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

UObject* UGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Equipment->GetInstigator();
	}
	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult UGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		Context.AddError(NSLOCTEXT("WarCollection", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}
	return Result;
}
#endif
