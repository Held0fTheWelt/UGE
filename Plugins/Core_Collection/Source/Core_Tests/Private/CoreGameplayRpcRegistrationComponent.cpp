// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "CoreGameplayRpcRegistrationComponent.h"
#include "GameFramework/PlayerController.h"
#include "Core_Classes/Public/Characters/CoreCharacter.h"
#include "Elemental_Player/Public/States/PlayerState_Elemental.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/GameInstance.h"
#include "Misc/CommandLine.h"
#include "EngineMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Core_AbilitySystem/Public/Components/CoreAbilitySystemComponent.h"
#include "Interfaces/HealthComponentInterface.h"
#include "Interfaces/InventoryManagerInterface.h"
#include "Interfaces/InventoryItemInstanceInterface.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

UCoreGameplayRpcRegistrationComponent* UCoreGameplayRpcRegistrationComponent::ObjectInstance = nullptr;
UCoreGameplayRpcRegistrationComponent* UCoreGameplayRpcRegistrationComponent::GetInstance()
{
#if WITH_RPC_REGISTRY
	if (ObjectInstance == nullptr)
	{
		ObjectInstance = NewObject<UCoreGameplayRpcRegistrationComponent>();
		FParse::Value(FCommandLine::Get(), TEXT("externalrpclistenaddress="), ObjectInstance->ListenerAddress);
		FParse::Value(FCommandLine::Get(), TEXT("rpcsenderid="), ObjectInstance->SenderID);
		if (!UExternalRpcRegistry::GetInstance())
		{
			GLog->Log(TEXT("BotRPC"), ELogVerbosity::Warning, FString::Printf(TEXT("Unable to create RPC Registry Instance. This might lead to issues using the RPC Registry.")));
		}
		ObjectInstance->AddToRoot();
	}
#endif
	return ObjectInstance;
}

UWorld* FindGameWorld()
{
	//Find Game World
	if (GEngine->GameViewport)
	{
		UGameInstance* GameInstance = GEngine->GameViewport->GetGameInstance();
		return GameInstance ? GameInstance->GetWorld() : nullptr;
	}
	return GWorld;
}

APlayerController* GetPlayerController()
{
	UWorld* LocalWorld = FindGameWorld();
	if (!LocalWorld)
	{
		return nullptr;
	}
	return LocalWorld->GetFirstPlayerController();
}

#if WITH_RPC_REGISTRY

TSharedPtr<FJsonObject> UCoreGameplayRpcRegistrationComponent::GetJsonObjectFromRequestBody(TArray<uint8> InRequestBody)
{
	FUTF8ToTCHAR WByteBuffer(reinterpret_cast<const ANSICHAR*>(InRequestBody.GetData()), InRequestBody.Num());
	const FString IncomingRequestBody =  FString::ConstructFromPtrSize(WByteBuffer.Get(), WByteBuffer.Length());
	TSharedPtr<FJsonObject> BodyObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(IncomingRequestBody);

	if (FJsonSerializer::Deserialize(JsonReader, BodyObject) && BodyObject.IsValid())
	{
		return BodyObject;
	}

	return nullptr;
}


void UCoreGameplayRpcRegistrationComponent::RegisterAlwaysOnHttpCallbacks()
{
	Super::RegisterAlwaysOnHttpCallbacks();	
	const FExternalRpcArgumentDesc CommandDesc(TEXT("command"), TEXT("string"), TEXT("The command to tell the executable to run."));

	RegisterHttpCallback(FName(TEXT("CheatCommand")),
		FHttpPath("/core/cheatcommand"),
		EHttpServerRequestVerbs::VERB_POST,
		FHttpRequestHandler::CreateUObject(this, &ThisClass::HttpExecuteCheatCommand),
		true,
		TEXT("Cheats"),
		TEXT("raw"),
		{ CommandDesc });
}

void UCoreGameplayRpcRegistrationComponent::RegisterInMatchHttpCallbacks()
{
	RegisterHttpCallback(FName(TEXT("GetPlayerStatus")),
		FHttpPath("/player/status"),
		EHttpServerRequestVerbs::VERB_GET,
		FHttpRequestHandler::CreateUObject(this, &ThisClass::HttpGetPlayerVitalsCommand),
		true);

	RegisterHttpCallback(FName(TEXT("PlayerFireOnce")),
		FHttpPath("/player/status"),
		EHttpServerRequestVerbs::VERB_POST,
		FHttpRequestHandler::CreateUObject(this, &ThisClass::HttpFireOnceCommand),
		true);
	
}

void UCoreGameplayRpcRegistrationComponent::RegisterFrontendHttpCallbacks()
{
	// Matchmaking RPCs for frontend (e.g. test/automation)
	RegisterHttpCallback(FName(TEXT("MatchmakingStatus")),
		FHttpPath("/matchmaking/status"),
		EHttpServerRequestVerbs::VERB_GET,
		FHttpRequestHandler::CreateUObject(this, &ThisClass::HttpGetMatchmakingStatus),
		true);

	RegisterHttpCallback(FName(TEXT("MatchmakingFind")),
		FHttpPath("/matchmaking/find"),
		EHttpServerRequestVerbs::VERB_POST,
		FHttpRequestHandler::CreateUObject(this, &ThisClass::HttpPostMatchmakingFind),
		true);
}


void UCoreGameplayRpcRegistrationComponent::DeregisterHttpCallbacks()
{
	Super::DeregisterHttpCallbacks();
}

bool UCoreGameplayRpcRegistrationComponent::HttpExecuteCheatCommand(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	TSharedPtr<FJsonObject> BodyObject = GetJsonObjectFromRequestBody(Request.Body);

	if (!BodyObject.IsValid())
	{
		TUniquePtr<FHttpServerResponse>Response = CreateSimpleResponse(false, TEXT("Invalid body object"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	if (BodyObject->GetStringField(TEXT("command")).IsEmpty())
	{
		TUniquePtr<FHttpServerResponse>Response = CreateSimpleResponse(false, TEXT("command not found in json body"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	APlayerController* LPC = GetPlayerController();
	if (!LPC)
	{
		TUniquePtr<FHttpServerResponse>Response = CreateSimpleResponse(false, TEXT("player controller not found"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	FString CheatCommand = FString::Printf(TEXT("%s"), *BodyObject->GetStringField(TEXT("command")));
	LPC->ConsoleCommand(*CheatCommand, true);

	TUniquePtr<FHttpServerResponse>Response = CreateSimpleResponse(true);
	OnComplete(MoveTemp(Response));
	return true;
}

bool UCoreGameplayRpcRegistrationComponent::HttpFireOnceCommand(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	APlayerController* LPC = GetPlayerController();
	if (!LPC)
	{
		TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("No player controller found"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	APawn* PlayerPawn = LPC->GetPawn();
	if (!PlayerPawn)
	{
		TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("Player pawn not found"));
		OnComplete(MoveTemp(Response));
		return true;
	}

	// Fire Once: trigger weapon/ability input via ASC if available
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PlayerPawn))
	{
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		{
			if (UCoreAbilitySystemComponent* CoreASC = Cast<UCoreAbilitySystemComponent>(ASC))
			{
				FGameplayTag FireTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Weapon.Fire")), false);
				if (FireTag.IsValid())
				{
					CoreASC->AbilityInputTagPressed(FireTag);
					CoreASC->AbilityInputTagReleased(FireTag);
				}
			}
		}
	}
	TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(true);
	OnComplete(MoveTemp(Response));
	return true;
}

bool UCoreGameplayRpcRegistrationComponent::HttpGetPlayerVitalsCommand(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	APlayerController* LPC = GetPlayerController();
	if (!LPC)
	{
		TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("No player controller found"));
		OnComplete(MoveTemp(Response));
		return true;
	}
	APawn* PlayerPawn = LPC->GetPawn();
	if (!PlayerPawn)
	{
		TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("Player pawn not found"));
		OnComplete(MoveTemp(Response));
		return true;
	}

	APlayerState_Elemental* PlayerState = LPC->GetPlayerState<APlayerState_Elemental>();
	if (!PlayerState)
	{
		TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(false, TEXT("Player state not found"));
		OnComplete(MoveTemp(Response));
		return true;
	}

	FString ResponseStr;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&ResponseStr);
	TSharedPtr<FJsonObject> BodyObject = MakeShareable(new FJsonObject());
	JsonWriter->WriteObjectStart();
	// Health via interface (no GameFeature_* dependency)
	for (UActorComponent* Comp : PlayerPawn->GetComponents())
	{
		if (IHealthComponentInterface* Health = Cast<IHealthComponentInterface>(Comp))
		{
			JsonWriter->WriteValue(TEXT("health"), FString::SanitizeFloat(Health->GetHealth()));
			break;
		}
	}
	// Inventory via interface (no GameFeature_* dependency)
	for (UActorComponent* Comp : PlayerPawn->GetComponents())
	{
		if (IInventoryManagerInterface* Inv = Cast<IInventoryManagerInterface>(Comp))
		{
			JsonWriter->WriteArrayStart(TEXT("inventory"));
			for (UObject* Item : Inv->GetInventoryItems())
			{
				if (Item)
				{
					IInventoryItemInstanceInterface* Inst = Cast<IInventoryItemInstanceInterface>(Item);
					JsonWriter->WriteObjectStart();
					JsonWriter->WriteValue(TEXT("def"), GetNameSafe(Inst ? Inst->GetItemDefinitionClass() : nullptr));
					JsonWriter->WriteValue(TEXT("instance"), GetNameSafe(Item));
					JsonWriter->WriteObjectEnd();
				}
			}
			JsonWriter->WriteArrayEnd();
			break;
		}
	}
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
	TUniquePtr<FHttpServerResponse>Response = FHttpServerResponse::Create(ResponseStr, TEXT("application/json"));
	OnComplete(MoveTemp(Response));
	return true;
}

bool UCoreGameplayRpcRegistrationComponent::HttpGetMatchmakingStatus(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	FString ResponseStr;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&ResponseStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("status"), TEXT("ok"));
	JsonWriter->WriteValue(TEXT("frontend"), true);
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
	TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(ResponseStr, TEXT("application/json"));
	OnComplete(MoveTemp(Response));
	return true;
}

bool UCoreGameplayRpcRegistrationComponent::HttpPostMatchmakingFind(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	// Placeholder: matchmaking find can be extended with body (e.g. match type, region)
	TUniquePtr<FHttpServerResponse> Response = CreateSimpleResponse(true);
	OnComplete(MoveTemp(Response));
	return true;
}

#endif
