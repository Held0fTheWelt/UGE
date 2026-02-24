// Copyright Epic Games, Inc. All Rights Reserved.

#include "Helicopters.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "HAL/IConsoleManager.h"
#include "Interfaces/DebugMassesAndCOMProviderInterface.h"

#define LOCTEXT_NAMESPACE "FHelicoptersModule"

namespace
{
	static int32 LastSMADebugLevel = 0;
}

void FHelicoptersModule::StartupModule()
{
	// heli.DebugCOM [0|1|2]: Physics body COM. 0=off, 1=on (name+mass), 2=verbose (+positions)
	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("heli.DebugCOM"),
		TEXT("Physics body COM debug. 0=off, 1=on, 2=verbose (with positions), no arg=toggle 0/1."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
		{
			IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("heli.DrawCOMDebug"));
			if (!CVar)
			{
				UE_LOG(LogTemp, Warning, TEXT("heli.DebugCOM: CVar heli.DrawCOMDebug not found (PhysicsBasedHelicopter plugin not loaded)."));
				return;
			}

			int32 Level = 0;
			if (Args.Num() > 0)
			{
				Level = FCString::Atoi(*Args[0]);
			}
			else
			{
				Level = (CVar->GetInt() == 0) ? 1 : 0;
			}

			CVar->Set(FMath::Clamp(Level, 0, 2));
			const TCHAR* StateStr = Level >= 2 ? TEXT("verbose") : (Level == 1 ? TEXT("on") : TEXT("off"));
			UE_LOG(LogTemp, Log, TEXT("heli.DebugCOM: Physics body COM debug %s (heli.DrawCOMDebug=%d)."), StateStr, Level);
		}),
		ECVF_Default);

	// sma.DebugCOM [0|1|2]: Per-mesh masses and hierarchy COM. 0=off, 1=on (name+mass), 2=verbose (+positions)
	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("sma.DebugCOM"),
		TEXT("Per-mesh mass/COM debug. 0=off, 1=on, 2=verbose (with positions), no arg=toggle 0/1."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
		{
			int32 Level = 0;
			if (Args.Num() > 0)
			{
				Level = FCString::Atoi(*Args[0]);
			}
			else
			{
				Level = (LastSMADebugLevel == 0) ? 1 : 0;
			}
			Level = FMath::Clamp(Level, 0, 2);
			LastSMADebugLevel = Level;

			UWorld* World = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr;
			if (!World) { UE_LOG(LogTemp, Warning, TEXT("sma.DebugCOM: No play world active.")); return; }

			// Verbose flag is communicated via a second Enable call with level encoding:
			// SetDebugMassesAndCOMEnabled(true) enables, SetDebugMassesAndCOMVerbose set separately below.
			const bool bEnable = Level > 0;

			// Set verbose CVar so the drawing code can read it
			IConsoleVariable* VerboseCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("sma.DrawCOMVerbose"));
			if (VerboseCVar)
			{
				VerboseCVar->Set(Level >= 2 ? 1 : 0);
			}

			int32 NumSet = 0;
			for (TActorIterator<AActor> It(World); It; ++It)
			{
				TArray<UActorComponent*> Comps = It->GetComponentsByInterface(UDebugMassesAndCOMProviderInterface::StaticClass());
				for (UActorComponent* Comp : Comps)
				{
					if (Comp) { IDebugMassesAndCOMProviderInterface::Execute_SetDebugMassesAndCOMEnabled(Comp, bEnable); ++NumSet; }
				}
			}
			const TCHAR* StateStr = Level >= 2 ? TEXT("verbose") : (Level == 1 ? TEXT("on") : TEXT("off"));
			UE_LOG(LogTemp, Log, TEXT("sma.DebugCOM: Per-mesh mass/COM debug %s (%d component(s))."), StateStr, NumSet);
		}),
		ECVF_Default);
}

void FHelicoptersModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHelicoptersModule, Helicopters)