// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Interfaces/IPluginManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace UGETTDShowcase
{
struct FContentPackProbe
{
	const TCHAR* PluginName;
	const TCHAR* ProbeAssetPath;
};

bool LoadJsonFile(const FString& RelativeProjectPath, TSharedPtr<FJsonObject>& OutJson, FString& OutError)
{
	const FString AbsolutePath = FPaths::Combine(FPaths::ProjectDir(), RelativeProjectPath);
	FString JsonText;
	if (!FFileHelper::LoadFileToString(JsonText, *AbsolutePath))
	{
		OutError = FString::Printf(TEXT("Could not read %s"), *AbsolutePath);
		return false;
	}

	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonText);
	if (!FJsonSerializer::Deserialize(Reader, OutJson) || !OutJson.IsValid())
	{
		OutError = FString::Printf(TEXT("Could not parse %s"), *AbsolutePath);
		return false;
	}

	return true;
}

int32 ReadIntField(const TSharedPtr<FJsonObject>& Object, const TCHAR* FieldName)
{
	return Object.IsValid() ? static_cast<int32>(Object->GetNumberField(FieldName)) : 0;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTTDShowcaseContentPackMountsTest,
	"Project.UGE.TTDShowcase.ContentPackMounts",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTTDShowcaseContentPackMountsTest::RunTest(const FString& Parameters)
{
	const UGETTDShowcase::FContentPackProbe RequiredPacks[] = {
		{ TEXT("LyraAudioAssets"), TEXT("Audio/DYN_LowMultibandDynamics.uasset") },
		{ TEXT("LyraCharacterAssets"), TEXT("Characters/Heroes/M_SimpleHeroPawn.uasset") },
		{ TEXT("LyraExampleContent"), TEXT("Effects/Material/M_Launcher.uasset") },
		{ TEXT("LyraMapAssets"), TEXT("Tools/SM_StairStep.uasset") },
		{ TEXT("LyraMaterialAssets"), TEXT("UserInterface/HUD/M_UI_RadialProgress_Ability.uasset") },
		{ TEXT("LyraSharedVisualAssets"), TEXT("Characters/Heroes/Mannequin/Textures/Shared/T_UE_Logo_V2.uasset") },
		{ TEXT("LyraSystemAssets"), TEXT("Feedback/Haptics/FFE_Weapon_Fire.uasset") },
		{ TEXT("LyraUIAssets"), TEXT("UI/Foundation/Materials/M_UI_Throbber_Base.uasset") },
		{ TEXT("LyraVfxAssets"), TEXT("Effects/Camera/Damage/NS_Screen_DamageDirection.uasset") },
		{ TEXT("LyraWeaponAssets"), TEXT("Items/PhysicsCube/SM_1M_Cube.uasset") }
	};

	for (const UGETTDShowcase::FContentPackProbe& RequiredPack : RequiredPacks)
	{
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(RequiredPack.PluginName);
		TestTrue(FString::Printf(TEXT("%s plugin is discoverable"), RequiredPack.PluginName), Plugin.IsValid());
		if (!Plugin.IsValid())
		{
			continue;
		}

		TestTrue(FString::Printf(TEXT("%s is enabled"), RequiredPack.PluginName), Plugin->IsEnabled());
		TestTrue(FString::Printf(TEXT("%s can contain content"), RequiredPack.PluginName), Plugin->CanContainContent());
		TestTrue(FString::Printf(TEXT("%s content dir exists"), RequiredPack.PluginName), FPaths::DirectoryExists(Plugin->GetContentDir()));

		const FString ProbePath = FPaths::Combine(Plugin->GetContentDir(), RequiredPack.ProbeAssetPath);
		TestTrue(FString::Printf(TEXT("%s probe asset exists"), RequiredPack.PluginName), FPaths::FileExists(ProbePath));
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTTDShowcaseBpjQueueContractTest,
	"Project.UGE.TTDShowcase.BpjQueueContract",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTTDShowcaseBpjQueueContractTest::RunTest(const FString& Parameters)
{
	TSharedPtr<FJsonObject> Manifest;
	TSharedPtr<FJsonObject> Queue;
	FString Error;

	TestTrue(TEXT("Showcase manifest loads"), UGETTDShowcase::LoadJsonFile(TEXT("Config/TTDShowcase/ttd-showcase-manifest.json"), Manifest, Error));
	if (!Manifest.IsValid())
	{
		AddError(Error);
		return false;
	}

	TestEqual(TEXT("Manifest schema"), Manifest->GetStringField(TEXT("schema")), FString(TEXT("ttd.uge_showcase_manifest.v1")));
	TestEqual(TEXT("Showcase role"), Manifest->GetStringField(TEXT("role")), FString(TEXT("showcase_project_for_ttd_plugin_output")));

	const TSharedPtr<FJsonObject>* ContentPackaging = nullptr;
	TestTrue(TEXT("Manifest has content_packaging"), Manifest->TryGetObjectField(TEXT("content_packaging"), ContentPackaging));
	if (ContentPackaging != nullptr && ContentPackaging->IsValid())
	{
		TestEqual(TEXT("Plan hash"), (*ContentPackaging)->GetStringField(TEXT("plan_hash")), FString(TEXT("8461d263d8d00eafda40e7af5016f5d013903a55")));
		TestEqual(TEXT("Pack count"), UGETTDShowcase::ReadIntField(*ContentPackaging, TEXT("pack_count")), 10);
		TestEqual(TEXT("Asset count"), UGETTDShowcase::ReadIntField(*ContentPackaging, TEXT("asset_count")), 1922);
		TestEqual(TEXT("No blockers"), UGETTDShowcase::ReadIntField(*ContentPackaging, TEXT("blocker_count")), 0);
		TestEqual(TEXT("No review queue"), UGETTDShowcase::ReadIntField(*ContentPackaging, TEXT("review_queue_count")), 0);
	}

	TestTrue(TEXT("BPJ rebuild queue loads"), UGETTDShowcase::LoadJsonFile(TEXT("Config/TTDShowcase/bpj-composition-rebuild-queue.json"), Queue, Error));
	if (!Queue.IsValid())
	{
		AddError(Error);
		return false;
	}

	TestEqual(TEXT("Queue schema"), Queue->GetStringField(TEXT("schema")), FString(TEXT("ttd.uge_bpj_composition_rebuild_queue.v1")));
	TestEqual(TEXT("Queue plan hash"), Queue->GetStringField(TEXT("plan_hash")), FString(TEXT("8461d263d8d00eafda40e7af5016f5d013903a55")));
	TestEqual(TEXT("Rebuild item count"), UGETTDShowcase::ReadIntField(Queue, TEXT("item_count")), 1244);
	TestEqual(TEXT("Rebuild group count"), UGETTDShowcase::ReadIntField(Queue, TEXT("group_count")), 8);

	const TArray<TSharedPtr<FJsonValue>>* Groups = nullptr;
	TestTrue(TEXT("Queue has groups"), Queue->TryGetArrayField(TEXT("groups"), Groups));
	if (Groups != nullptr)
	{
		TSet<FString> Architectures;
		for (const TSharedPtr<FJsonValue>& GroupValue : *Groups)
		{
			const TSharedPtr<FJsonObject> GroupObject = GroupValue.IsValid() ? GroupValue->AsObject() : nullptr;
			if (GroupObject.IsValid())
			{
				Architectures.Add(GroupObject->GetStringField(TEXT("target_architecture")));
			}
		}

		TestTrue(TEXT("AnimationComposition group exists"), Architectures.Contains(TEXT("AnimationComposition")));
		TestTrue(TEXT("GameplayComposition group exists"), Architectures.Contains(TEXT("GameplayComposition")));
		TestTrue(TEXT("UIComposition group exists"), Architectures.Contains(TEXT("UIComposition")));
		TestTrue(TEXT("MapAssembly group exists"), Architectures.Contains(TEXT("MapAssembly")));
		TestTrue(TEXT("EditorTooling discard group exists"), Architectures.Contains(TEXT("EditorTooling")));
	}

	return true;
}

#endif
