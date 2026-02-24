// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CommonGameViewportClient.h"

#include "CoreGameViewportClient.generated.h"

class UGameInstance;
class UObject;

UCLASS(BlueprintType)
class CORE_CLASSES_API UCoreGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	UCoreGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
