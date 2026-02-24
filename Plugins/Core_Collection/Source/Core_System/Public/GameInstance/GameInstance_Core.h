// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CommonGameInstance.h"
#include "Interfaces/ExperienceLoadedListenerInterface.h"
#include "Interfaces/ReplaySupportProviderInterface.h"
#include "GameInstance/CoreGameInstanceDelegates.h"
#include "GameInstance_Core.generated.h"

/**
 * Game instance. Implements Elemental interfaces for Experience system communication.
 * Core_Setup and Core_Replay register via CoreGameInstanceDelegates (Core_Interfaces); this class invokes/queries there.
 */
UCLASS(Config = Game)
class CORE_SYSTEM_API UGameInstance_Core : public UCommonGameInstance, public IExperienceLoadedListenerInterface, public IReplaySupportProviderInterface
{
	GENERATED_BODY()

public:

	UGameInstance_Core(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~IExperienceLoadedListenerInterface
	UFUNCTION(BlueprintCallable, Category = "Experience")
	void OnExperienceLoaded(const UObject* LoadedExperience) override;

	//~IReplaySupportProviderInterface
	UFUNCTION(BlueprintCallable, Category = "Replay")
	bool DoesPlatformSupportReplays() const override;

private:
	//ALyraPlayerController* GetPrimaryPlayerController() const;

	virtual bool CanJoinRequestedSession() const override;
	virtual void HandlerUserInitialized(const class UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext) override;

	virtual void ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate) override;
	virtual void ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate) override;

protected:

	virtual void Init() override;
	virtual void Shutdown() override;

	void OnPreClientTravelToSession(FString& URL);

	/** A hard-coded encryption key used to try out the encryption code. This is NOT SECURE, do not use this technique in production! */
	TArray<uint8> DebugTestEncryptionKey;
};

#undef UE_API