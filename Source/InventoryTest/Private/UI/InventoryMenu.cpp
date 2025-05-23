// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryMenu.h"

#include "Components/Image.h"
#include "Components/PanelWidget.h"

void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetInternalBlueprintReferences();

	SetVisibility(ESlateVisibility::Visible);
	for (UImage * ItemImage : ItemImages)
	{
		ItemImage->SetBrushFromTexture(nullptr, false);
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryMenu::SetItemImage(int ItemSlotIndex, UTexture2D* NewItemImage)
{
	if (ItemImages.IsValidIndex(ItemSlotIndex) && ItemImages[ItemSlotIndex])
	{
		ItemImages[ItemSlotIndex]->SetBrushFromTexture(NewItemImage, false);
		ItemImages[ItemSlotIndex]->SetVisibility(NewItemImage ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
