// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetManager/AssetManagerStartupJob_Core.h"

DEFINE_LOG_CATEGORY(Log_AssetManagerStartupJob);

TSharedPtr<FStreamableHandle> FAssetManagerStartupJob_Core::DoJob() const
{
	const double JobStartTime = FPlatformTime::Seconds();

	TSharedPtr<FStreamableHandle> Handle;
	UE_LOG(Log_AssetManagerStartupJob, Display, TEXT("Startup job \"%s\" starting"), *JobName);
	JobFunc(*this, Handle);

	if (Handle.IsValid())
	{
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateRaw(this, &FAssetManagerStartupJob_Core::UpdateSubstepProgressFromStreamable));
		Handle->WaitUntilComplete(0.0f, false);
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate());
	}

	UE_LOG(Log_AssetManagerStartupJob, Display, TEXT("Startup job \"%s\" took %.2f seconds to complete"), *JobName, FPlatformTime::Seconds() - JobStartTime);

	return Handle;
}