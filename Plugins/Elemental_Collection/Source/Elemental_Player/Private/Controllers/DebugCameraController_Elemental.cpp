// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DebugCameraController_Elemental.h"

//#include "LyraCheatManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DebugCameraController_Elemental)

ADebugCameraController_Elemental::ADebugCameraController_Elemental(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Use the same cheat class as LyraPlayerController to allow toggling the debug camera through cheats.
	//CheatClass = ULyraCheatManager::StaticClass();
}

void ADebugCameraController_Elemental::AddCheats(bool bForce)
{
	// Mirrors LyraPlayerController's AddCheats() to avoid the player becoming stuck in the debug camera.
#if USING_CHEAT_MANAGER
	Super::AddCheats(true);
#else //#if USING_CHEAT_MANAGER
	Super::AddCheats(bForce);
#endif // #else //#if USING_CHEAT_MANAGER
}