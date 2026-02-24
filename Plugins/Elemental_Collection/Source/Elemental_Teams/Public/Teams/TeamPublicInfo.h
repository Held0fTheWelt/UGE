// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Teams/TeamInfoBase.h"

#include "TeamPublicInfo.generated.h"

class UTeamCreationComponent;
class UTeamDisplayAsset;
class UObject;
struct FFrame;

UCLASS()
class ELEMENTAL_TEAMS_API ATeamPublicInfo : public ATeamInfoBase
{
	GENERATED_BODY()

	friend UTeamCreationComponent;

public:
	ATeamPublicInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	void SetTeamDisplayAsset(TObjectPtr<UTeamDisplayAsset> NewDisplayAsset);

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamDisplayAsset)
	TObjectPtr<UTeamDisplayAsset> TeamDisplayAsset;
};
