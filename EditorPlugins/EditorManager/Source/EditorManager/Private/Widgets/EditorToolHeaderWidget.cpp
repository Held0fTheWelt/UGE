// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EditorToolHeaderWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UEditorToolHeaderWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Text_EditorToolHeader)
	{
		Text_EditorToolHeader->SetText(EditorToolHeaderText);
	}

	if (Text_EditorToolHeaderVersion)
	{
		Text_EditorToolHeaderVersion->SetText(EditorToolVersionText);
	}

	if (Image_EditorToolHeader)
	{
		if(EditorToolHeaderImage.IsNull())
		{
			return;
		}
		else
		{
			if(!EditorToolHeaderImage.IsValid())
			{
				EditorToolHeaderImage.LoadSynchronous();
			}

			Image_EditorToolHeader->SetBrushFromTexture(EditorToolHeaderImage.Get());
		}
	}
}
