// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GuidManageSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_Subsystem_GuidManager, Log, All);
/**
 * 
 */
UCLASS()
class CORE_CLASSES_API UGuidManageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	UGuidManageSubsystem();

	//// Function to retrieve a GUID entry by its path
	bool GetGuidEntryByPath(const FString& MapName, const FString& ObjectPath, FGuid& OutObjectGUID) const;
	//// Function to register a new GUID entry
	//void RegisterGuidEntry(const FString& MapName, const FGuid& ObjectGUID, const FString& ObjectPath);
protected:
	// Called when the game instance is initialized
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// Called when the game instance is deinitialized
	virtual void Deinitialize() override;

};
