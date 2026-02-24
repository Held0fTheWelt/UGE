// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Subsystems/CommonMessagingSubsystem.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPtr.h"

#include "CoreUIMessagingSubsystem.generated.h"

class FSubsystemCollectionBase;
class UCommonGameDialog;
class UCommonGameDialogDescriptor;
class UObject;

/**
 * UCoreUIMessagingSubsystem
 *
 * UI Messaging Subsystem for Core.
 * Provides unified dialog management (Confirmation, Error, Notification).
 * Adapted from ULyraUIMessaging.
 */
UCLASS()
class CORE_CLASSES_API UCoreUIMessagingSubsystem : public UCommonMessagingSubsystem
{
	GENERATED_BODY()

public:
	UCoreUIMessagingSubsystem() { }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void ShowConfirmation(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate()) override;
	virtual void ShowError(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate()) override;

private:
	UPROPERTY()
	TSubclassOf<UCommonGameDialog> ConfirmationDialogClassPtr;

	UPROPERTY()
	TSubclassOf<UCommonGameDialog> ErrorDialogClassPtr;

	UPROPERTY(config)
	TSoftClassPtr<UCommonGameDialog> ConfirmationDialogClass;

	UPROPERTY(config)
	TSoftClassPtr<UCommonGameDialog> ErrorDialogClass;
};
