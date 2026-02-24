// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CombinedBundleUserWidget.h"

#include "AssetManager/AssetManagedLoaderHelper.h"
#include "AssetManager/AsyncAssetLoaderHelper.h"

void UCombinedBundleUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(AssetsToLoad.Num() > 0 || ClassesToLoad.Num() > 0)
	{
		UAsyncAssetLoaderHelper* Loader = UAssetManagedLoaderHelper::LoadObjectsAndClassesAsync(
			this,
			AssetsToLoad,
			ClassesToLoad
		);

		Loader->OnAllAssetsLoaded.AddDynamic(
			this,
			&UCombinedBundleUserWidget::AllAsyncAssetsLoaded
		);
	}
}

void UCombinedBundleUserWidget::AllAsyncAssetsLoaded()
{

}