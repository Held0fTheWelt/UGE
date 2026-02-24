// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Subsystems/LocalPlayerSubsystem.h"

#include "Enums/CommonMessagingResult.h"
#include "CommonMessagingSubsystem.generated.h"

#define UE_API ELEMENTAL_COMMON_API

class FSubsystemCollectionBase;
class UCommonGameDialogDescriptor;
class UObject;

DECLARE_DELEGATE_OneParam(FCommonMessagingResultDelegate, ECommonMessagingResult /* Result */);

/**
 *
 */
UCLASS(MinimalAPI, config = Game)
class UCommonMessagingSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UCommonMessagingSubsystem() {}

	UE_API virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UE_API virtual void Deinitialize() override;
	UE_API virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UE_API virtual void ShowConfirmation(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate());
	UE_API virtual void ShowError(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate());

private:

};

#undef UE_API
