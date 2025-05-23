// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ActiveInventorySlot.h"

#include "Components/Image.h"

void UActiveInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	SetInternalBlueprintReferences();

	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(nullptr, false);
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UActiveInventorySlot::SetItemImage(UTexture2D* NewItemImage)
{
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(NewItemImage, false);
		ItemImage->SetVisibility(NewItemImage ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

