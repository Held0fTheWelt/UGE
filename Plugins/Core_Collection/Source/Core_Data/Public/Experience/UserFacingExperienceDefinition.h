// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "UserFacingExperienceDefinition.generated.h"

class UCommonSession_HostSessionRequest;
class UObject;

/**
 * User-facing experience definition for session creation (e.g. lobby). Uses Elemental interfaces for replay support.
 */
UCLASS(BlueprintType)
class CORE_DATA_API UUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "ExperienceDefinition"))
	FPrimaryAssetId ExperienceID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience)
	FPrimaryAssetId MapID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience)
	TMap<FString, FString> ExtraArgs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience)
	int32 MaxPlayerCount = 16;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience)
	bool bRecordReplay = false;

	UFUNCTION(BlueprintCallable, Category = Experience)
	UCommonSession_HostSessionRequest* CreateHostingRequest(const UObject* WorldContextObject) const;
};
