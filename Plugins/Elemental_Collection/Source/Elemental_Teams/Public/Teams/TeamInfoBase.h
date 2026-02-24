// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "GameFramework/Info.h"
#include "System/GameplayTagStack.h"

#include "TeamInfoBase.generated.h"

namespace EEndPlayReason { enum Type : int; }

class UTeamCreationComponent;
class UTeamSubsystem;
class UObject;
struct FFrame;

UCLASS(Abstract)
class ELEMENTAL_TEAMS_API ATeamInfoBase : public AInfo
{
	GENERATED_BODY()

public:
	ATeamInfoBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	int32 GetTeamId() const { return TeamId; }

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of AActor interface

protected:
	virtual void RegisterWithTeamSubsystem(UTeamSubsystem* Subsystem);
	void TryRegisterWithTeamSubsystem();

private:
	void SetTeamId(int32 NewTeamId);

	UFUNCTION()
	void OnRep_TeamId();

public:
	friend UTeamCreationComponent;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TeamTags;

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamId)
	int32 TeamId;
};
