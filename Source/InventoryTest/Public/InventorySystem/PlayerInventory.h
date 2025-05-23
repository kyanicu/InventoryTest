// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemPickUp.h"
#include "UObject/Object.h"
#include "PlayerInventory.generated.h"

class AInventoryTestPlayerController;
class UInventoryMenu;
class UActiveInventorySlot;
class UPlayerHUD;
class UImage;
class UTexture;
/**
 * 
 */
UCLASS()
class INVENTORYTEST_API UPlayerInventory : public UObject
{
	GENERATED_BODY()

public:

	void Initialize(UPlayerHUD* HUD);
	
	void PickUpItem(AItemPickUp* Item);
	void ForgetItem(int SlotIndexToForget);
	
	void TrackDraggedItemIndex(int SlotIndex);
	void DropDraggedItemIntoSlot(int SlotToDropInIndex);
	void StopTrackingDraggedItemIndex();
	int GetDraggedItemIndex() const { return CurrentDraggedItem; }
	
	int GetMouseHoveredInventorySlot(FVector2D MouseScreenSpacePosition, bool bCheckEmptySlots);
	UImage* GetItemSlotImage(int SlotIndex, bool bReturnEvenIfEmptyItem);
	UImage* GetDraggedItemImage();

	// -1 = Active Slot, >=0 = inventory slots
	FItemData GetItemData(int ItemSlotIndex); 
	
	void SetItemImage(int SlotIndex, UTexture2D* Texture);
	
private:

	bool CheckMouseHoveredOverInventorySlotImage(FVector2D MouseScreenSpacePosition, UImage* SlotImage);
	
	UPROPERTY()
	UActiveInventorySlot* ActiveInventorySlot;

	UPROPERTY()
	UInventoryMenu* InventoryMenu;

	// Might make code cleaner if I fit this into InventoryItems as either 0 or InventorySize? 
	FItemData CurrentActiveItem;

	TArray<FItemData> InventoryItems;

	// Make this customizable instead of a hard coded const 
	const int InventorySize = 4;

	bool bDraggingItem = false;
	int CurrentDraggedItem = -2; // -2 = invalid, -1 = Active Slot, >=0 = inventory slots
	// TODO: Might be cleaner to have active slot be max size, and invalid be -1 as usual
};