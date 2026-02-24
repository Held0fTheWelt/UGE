// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Teams/TeamInfoBase.h"

#include "TeamPrivateInfo.generated.h"

class UObject;

UCLASS()
class ELEMENTAL_TEAMS_API ATeamPrivateInfo : public ATeamInfoBase
{
	GENERATED_BODY()

public:
	ATeamPrivateInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
