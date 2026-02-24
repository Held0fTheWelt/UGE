/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        ActorUIImagePrimaryDataAsset.h
 * Created:     2025-06-12
 * Description: Primary data asset for associating an image/texture and display parameters
 *              with an Actor for in-world UMG widgets (e.g. health bar, icon, status).
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "ActorUIImageDataAsset.generated.h"

 /**
  * UActorUIImagePrimaryDataAsset
  *
  * Primary Data Asset for actor UI images, providing parameters for in-world widget display
  * (e.g. health bar, icon, overlay) such as size, attachment point, image, and scaling.
  *
  * Use this asset as a source for consistent, design-driven UI element setup per actor.
  */
UCLASS(BlueprintType)
class ELEMENTAL_DATA_API UActorUIImageDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	/** Standard constructor, sets default display parameters for actor UI elements. */
	UActorUIImageDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
		, Height(50.0f)
		, Image(nullptr)
		, Size(FVector2D(200.0f, 50.0f))
		, NameOfSceneComponentToAttach(TEXT("Mesh"))
		, WidgetScale(FVector(0.5f, 0.5f, 0.5f))
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	/** Default height for the UI widget (e.g. vertical offset for health bar) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float Height;

	/** The image (texture) to be used for the widget (health bar background, icon, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftObjectPtr<class UTexture2D> Image;

	/** Default widget size (width x height in screen units or pixels) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D Size;

	/** Name of the scene component to which this widget should be attached (e.g. Mesh, Head) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FName NameOfSceneComponentToAttach;

	/** Widget scale to be applied (XYZ, usually for non-uniform or in-world scaling) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector WidgetScale;

	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

	virtual TArray<TSoftObjectPtr<UObject>> GetAssetObjects_Implementation() const override
	{
		TArray<TSoftObjectPtr<UObject>> AssetObjects;
		
		if(Image.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
				"{Time}: {Line} {Class}: Image is null in UActorUIImageDataAsset. No asset objects to return.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}
		else
		{
			AssetObjects.Add(Image);
		}

		return AssetObjects;
	}

private:
	/** Internal static asset type (define value in .cpp for asset manager integration) */
	static const FPrimaryAssetType AssetType;
};
