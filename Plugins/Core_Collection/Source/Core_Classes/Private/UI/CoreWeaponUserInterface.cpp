// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/CoreWeaponUserInterface.h"

#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreWeaponUserInterface)

struct FGeometry;

UCoreWeaponUserInterface::UCoreWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCoreWeaponUserInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCoreWeaponUserInterface::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCoreWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// Current weapon is set from outside via SetCurrentWeapon (e.g. by GameFeature using IEquipmentManagerInterface).
}

void UCoreWeaponUserInterface::SetCurrentWeapon(UObject* InWeapon)
{
	if (InWeapon != CurrentInstance)
	{
		UObject* OldWeapon = CurrentInstance;
		CurrentInstance = InWeapon;
		RebuildWidgetFromWeapon();
		OnWeaponChanged(OldWeapon, CurrentInstance);
	}
}

void UCoreWeaponUserInterface::RebuildWidgetFromWeapon()
{
	// Override in Blueprint to rebuild widget based on weapon
}
