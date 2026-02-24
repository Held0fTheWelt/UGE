// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GameFeaturedLevelScriptActor.h"

#include "Components/GameFrameworkComponentManager.h"
#include "Settings/CoreSettingsLocal.h"

#include "Logging/Core_Classes_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"



void AGameFeaturedLevelScriptActor::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AGameFeaturedLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();	

	// Add the game feature listener if it was not added yet
	AddGameFeatureListener();
}

void AGameFeaturedLevelScriptActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Remove the game feature listener if it was added
	RemoveGameFeatureListener();
}

FName AGameFeaturedLevelScriptActor::GetDefinitionAssetName_Implementation() const
{
	return GetFName();
}

#pragma region GameFeatures
bool AGameFeaturedLevelScriptActor::AddGameFeatureListener()
{
	if (bGameFeatureInitialized)
	{
		// Log that the game feature listener is not initialized
		UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: Game feature listener is already initialized for GameFeaturedLevelScriptActor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", GetFName().ToString()));
		return false;
	}

	bGameFeatureInitialized = true;


	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	// Log addition of the game feature listener
	UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: Added game feature listener to GameFeaturedLevelScriptActor: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	return false;
}
bool AGameFeaturedLevelScriptActor::RemoveGameFeatureListener()
{
	if (!bGameFeatureInitialized)
	{
		// Log that the game feature listener is not initialized
		UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: Game feature listener is not initialized for GameFeaturedLevelScriptActor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", GetFName().ToString()));
		return false;
	}

	bGameFeatureInitialized = false;

	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	// Log removal of the game feature listener
	UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: Removed game feature listener from GameFeaturedLevelScriptActor: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	return true;
}
bool AGameFeaturedLevelScriptActor::ShouldIgnoreGameFeature_Implementation(const FString& GameFeature) const
{
	// PlanesInTheSky: zusätzlich Spiel-Einstellung prüfen (Checkbox in Einstellungen)
	if (GameFeature == TEXT("PlanesInTheSky"))
	{
		if (UCoreSettingsLocal* LocalSettings = UCoreSettingsLocal::Get())
		{
			if (!LocalSettings->IsPlanesInTheSkyEnabled())
			{
				UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: PlanesInTheSky disabled by user setting.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
				return true;
			}
		}
	}

	// Log the check for ignoring a game feature
	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: AGameFeaturedLevelScriptActor should ignore game feature {GameFeature} : {Result}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("GameFeature", GameFeature),
		("Result", IgnoredGameFeatures.Contains(GameFeature) ? TEXT("True") : TEXT("False"))
	);
	return IgnoredGameFeatures.Contains(GameFeature);
}
#pragma endregion GameFeatures