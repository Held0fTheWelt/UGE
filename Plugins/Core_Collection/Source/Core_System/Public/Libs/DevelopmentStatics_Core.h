// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "DevelopmentStatics_Core.generated.h"

/**
 * 
 */
UCLASS()
class CORE_SYSTEM_API UDevelopmentStatics_Core : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Should game logic skip directly to gameplay (skipping any match warmup / waiting for players / etc... aspects)
	// Will always return false except when playing in the editor and bTestFullGameFlowInPIE (in Lyra Developer Settings) is false
	UFUNCTION(BlueprintCallable, Category = "Testing|Dev")
	static bool ShouldSkipDirectlyToGameplay();

	// Should game logic load cosmetic backgrounds in the editor?
	// Will always return true except when playing in the editor and bSkipLoadingCosmeticBackgroundsInPIE (in Lyra Developer Settings) is true
	UFUNCTION(BlueprintCallable, Category = "Testing|Dev", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool ShouldLoadCosmeticBackgrounds();

	// Should game logic load cosmetic backgrounds in the editor?
	// Will always return true except when playing in the editor and bSkipLoadingCosmeticBackgroundsInPIE (in Lyra Developer Settings) is true
	UFUNCTION(BlueprintCallable, Category = "Testing|Dev")
	static bool CanPlayerBotsAttack();

	// Finds the most appropriate play-in-editor world to run 'server' cheats on
	//   This might be the only world if running standalone, the listen server, or the dedicated server
	static UWorld* FindPlayInEditorAuthorityWorld();

	/** Runs the given lambda on every authority world (PIE/Game worlds that are not client-only). Use for multi-world setups. */
	template<typename Fn>
	static void ForEachAuthorityWorld(Fn&& Lambda)
	{
		if (!GEngine) { return; }
		for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
		{
			UWorld* World = WorldContext.World();
			if (!World) { continue; }
			if (World->GetNetMode() == NM_Client) { continue; }
			if (WorldContext.WorldType != EWorldType::PIE && WorldContext.WorldType != EWorldType::Game) { continue; }
			Lambda(World);
		}
	}

	// Tries to find a class by a short name (with some heuristics to improve the usability when done via a cheat console)
	static UClass* FindClassByShortName(const FString& SearchToken, UClass* DesiredBaseClass, bool bLogFailures = true);

	template <typename DesiredClass>
	static TSubclassOf<DesiredClass> FindClassByShortName(const FString& SearchToken, bool bLogFailures = true)
	{
		return FindClassByShortName(SearchToken, DesiredClass::StaticClass(), bLogFailures);
	}

private:
	static TArray<FAssetData> GetAllBlueprints();
	static UClass* FindBlueprintClass(const FString& TargetNameRaw, UClass* DesiredBaseClass);
};
