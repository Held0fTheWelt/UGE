// Copyright WarCollection, All Rights Reserved.

using UnrealBuildTool;

public class GameFeature_AccoladeSystemRuntime : ModuleRules
{
	public GameFeature_AccoladeSystemRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"GameplayTags",				// FGameplayTag, NativeGameplayTags
			"GameplayMessageRuntime",	// UGameplayMessageSubsystem, FGameplayMessageListenerHandle
			"Elemental_MessageSystem",	// UGameplayMessageProcessor, FNotificationMessage, UVerbMessageHelpers
			"Elemental_Structure",		// FVerbMessage (Structs/VerbMessageTypes.h)
			"CommonUI",					// UCommonUserWidget
			"UMG",						// UUserWidget
			"DataRegistry",				// UDataRegistrySubsystem, FDataRegistryAcquireResult
			"AsyncMixin",				// FAsyncMixin
		});
	}
}
