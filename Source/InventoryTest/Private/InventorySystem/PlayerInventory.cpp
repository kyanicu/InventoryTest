// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/PlayerInventory.h"

#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "InventorySystem/ItemPickUp2D.h"
#include "InventorySystem/ItemPickUp3D.h"
#include "UI/ActiveInventorySlot.h"
#include "UI/InventoryMenu.h"
#include "UI/PlayerHUD.h"

class AItemPickUp3D;

void UPlayerInventory::Initialize(UPlayerHUD* HUD)
{
	if (HUD)
	{
		InventoryMenu = HUD->GetInventoryMenu();
		ActiveInventorySlot = HUD->GetActiveInventorySlot();
	}

	InventoryItems.SetNumZeroed(InventorySize);
}

void UPlayerInventory::PickUpItem(AItemPickUp* Item)
{
	int SlotToFill;
	if (CurrentActiveItem.ItemName == NAME_None)
	{
		SlotToFill = -1;
	}
	else
	{
		// Find first empty slot
		// Better way than looping? Maybe make this work as a stack?
		for (SlotToFill = 0; SlotToFill < InventorySize && InventoryItems[SlotToFill].ItemName != NAME_None; SlotToFill++){}
		
		if (SlotToFill >= InventorySize)
		{
			return;
		}
	}
	
	FItemData ItemData = Item->GetItemData();
	if (AItemPickUp2D* Item2D = Cast<AItemPickUp2D>(Item))
	{
		UTexture2D* ItemImage = Item2D->GetItemImage();
		if (SlotToFill == -1)
		{
			ActiveInventorySlot->SetItemImage(ItemImage);
			CurrentActiveItem = ItemData;
		}
		else
		{
			InventoryMenu->SetItemImage(SlotToFill, ItemImage);
			InventoryItems[SlotToFill] = ItemData;
		}
		
		Item2D->Destroy();
	}
	else if (AItemPickUp3D* Item3D = Cast<AItemPickUp3D>(Item))
	{
		if (!Item3D->GetIsInventoryItem())
		{
			if (SlotToFill == -1)
			{
				Item3D->InitializeAsMenuItem(SlotToFill, ActiveInventorySlot->GetItemImage()->GetParent(), false);
				CurrentActiveItem = ItemData;
			}
			else
			{
				Item3D->InitializeAsMenuItem(SlotToFill, InventoryMenu->GetItemImages()[SlotToFill]->GetParent(), InventoryMenu->GetVisibility() == ESlateVisibility::Hidden);
				InventoryItems[SlotToFill] = ItemData;
			}
		}
	}
}

void UPlayerInventory::ForgetItem(int SlotIndexToForget)
{
	if (SlotIndexToForget == -1)
	{
		CurrentActiveItem = FItemData();
	}
	else if (InventoryItems.IsValidIndex(SlotIndexToForget))
	{
		InventoryItems[SlotIndexToForget] = FItemData();
	}
}

void UPlayerInventory::TrackDraggedItemIndex(int SlotIndex)
{
	bDraggingItem = true;
	CurrentDraggedItem = SlotIndex;
}

void UPlayerInventory::DropDraggedItemIntoSlot(int SlotToDropInIndex)
{
	if (SlotToDropInIndex == -1)
	{
		FItemData SwapHold = CurrentActiveItem; 
		CurrentActiveItem = InventoryItems[CurrentDraggedItem];
		InventoryItems[CurrentDraggedItem] = SwapHold;
	}
	else if (CurrentDraggedItem == -1)
	{
		FItemData SwapHold = CurrentActiveItem; 
		CurrentActiveItem = InventoryItems[SlotToDropInIndex];
		InventoryItems[SlotToDropInIndex] = SwapHold;
	}
	else if (InventoryItems.IsValidIndex(SlotToDropInIndex))
	{
		InventoryItems.Swap(CurrentDraggedItem, SlotToDropInIndex);
	}
	
	StopTrackingDraggedItemIndex();
}

void UPlayerInventory::StopTrackingDraggedItemIndex()
{
	bDraggingItem = false;
	CurrentDraggedItem = -2;
}

int UPlayerInventory::GetMouseHoveredInventorySlot(FVector2D MouseScreenSpacePosition, bool bCheckEmptySlots)
{
	if ((bCheckEmptySlots || CurrentActiveItem.ItemName != NAME_None)
		&& CheckMouseHoveredOverInventorySlotImage(MouseScreenSpacePosition, ActiveInventorySlot->GetItemImage()))
	{
		return -1;
	}
	
	for (int FoundSlotIndex = 0; FoundSlotIndex < InventorySize; FoundSlotIndex++)
	{
		if ((bCheckEmptySlots || InventoryItems[FoundSlotIndex].ItemName != NAME_None)
			&& CheckMouseHoveredOverInventorySlotImage(MouseScreenSpacePosition, InventoryMenu->GetItemImages()[FoundSlotIndex]))
		{
			return FoundSlotIndex;
		}
	}
	
	return -2; // Invalid
}

UImage* UPlayerInventory::GetItemSlotImage(int SlotIndex, bool bReturnEvenIfEmptyItem)
{
	if (SlotIndex == -1 && (CurrentActiveItem.ItemName != NAME_None || bReturnEvenIfEmptyItem)) 
	{
		return ActiveInventorySlot->GetItemImage();
	}

	if (InventoryItems.IsValidIndex(SlotIndex) && (InventoryItems[SlotIndex].ItemName != NAME_None || bReturnEvenIfEmptyItem))
	{
		return InventoryMenu->GetItemImages()[SlotIndex];
	}

	return nullptr;
}

UImage* UPlayerInventory::GetDraggedItemImage()
{
	return GetItemSlotImage(CurrentDraggedItem, false);
}

FItemData UPlayerInventory::GetItemData(int ItemSlotIndex)
{
	if (ItemSlotIndex == -1 && CurrentActiveItem.ItemName != NAME_None)
	{
		return CurrentActiveItem;
	}

	if (InventoryItems.IsValidIndex(ItemSlotIndex) && InventoryItems[ItemSlotIndex].ItemName != NAME_None)
	{
		return InventoryItems[ItemSlotIndex];
	}

	return FItemData();
}

void UPlayerInventory::SetItemImage(int SlotIndex, UTexture2D* Texture)
{
	if (SlotIndex == -1)
	{
		ActiveInventorySlot->SetItemImage(Texture);
	}
	else if (InventoryItems.IsValidIndex(SlotIndex))
	{
		InventoryMenu->SetItemImage(SlotIndex, Texture);
	}
}

// TODO: Make the code inside a helper function that can be accessed anywhere for simple detection of any widget
bool UPlayerInventory::CheckMouseHoveredOverInventorySlotImage(FVector2D MouseScreenSpacePosition, UImage* SlotImage)
{
	const FGeometry ImageGeometry = SlotImage->GetParent()->GetCachedGeometry(); // Check parent Border widget incase image is empty
	const FVector2D MouseLocalPosition = ImageGeometry.AbsoluteToLocal(MouseScreenSpacePosition);

	const FVector2D Size = ImageGeometry.GetLocalSize();
	const bool bHoveredOver =
		MouseLocalPosition.X >= 0.f && MouseLocalPosition.X <= Size.X &&
		MouseLocalPosition.Y >= 0.f && MouseLocalPosition.Y <= Size.Y;

	return bHoveredOver;
}
