// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Teams/TeamPublicInfo.h"

#include "Net/UnrealNetwork.h"
#include "Teams/TeamInfoBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TeamPublicInfo)

class FLifetimeProperty;

ATeamPublicInfo::ATeamPublicInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ATeamPublicInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, TeamDisplayAsset, COND_InitialOnly);
}

void ATeamPublicInfo::SetTeamDisplayAsset(TObjectPtr<UTeamDisplayAsset> NewDisplayAsset)
{
	check(HasAuthority());
	check(TeamDisplayAsset == nullptr);

	TeamDisplayAsset = NewDisplayAsset;

	TryRegisterWithTeamSubsystem();
}

void ATeamPublicInfo::OnRep_TeamDisplayAsset()
{
	TryRegisterWithTeamSubsystem();
}
