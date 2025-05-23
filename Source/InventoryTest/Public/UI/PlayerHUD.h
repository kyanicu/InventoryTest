// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UButton;
class UBorder;
class AItemPickUp3D;
class UCanvasPanelSlot;
class UPlayerInventory;
class UActiveInventorySlot;
class UInventoryMenu;
class UCanvasPanel;
class UImage;
class AItemPickUp3D;
class UPanelWidget;

// For any info needed to handle 3D items in inventory slots
USTRUCT(BlueprintType)
struct F3DItemInventorySlotInfo
{
	GENERATED_BODY()

	int InventorySlot; //-1 is active slot

	float InventorySlotScale;

	UPROPERTY()
	UPanelWidget* WidgetToTrack;
};

/**
 * 
 */
UCLASS()
class INVENTORYTEST_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void OpenInventory();

	UFUNCTION()
	void CloseInventory();
	
	UActiveInventorySlot* GetActiveInventorySlot() const { return ActiveInventorySlot; }
	UInventoryMenu* GetInventoryMenu() const { return InventoryMenu; }

	bool GetIsInventoryOpen() const { return bInventoryOpen; }

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void SetPlayerInventory(UPlayerInventory* Inventory) { PlayerInventory = Inventory; }

	void Track3DItem(AItemPickUp3D* Item, int InventorySlot, UPanelWidget* WidgetToTrack);
	void Update3DItemTransform(AItemPickUp3D* Item);
	
protected:

	UFUNCTION(BlueprintImplementableEvent)
	void SetInternalBlueprintReferences();

	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* CanvasPanel;

	UPROPERTY(BlueprintReadWrite)
	UActiveInventorySlot* ActiveInventorySlot;

	UPROPERTY(BlueprintReadWrite)
	UInventoryMenu* InventoryMenu;
	
	UPROPERTY(BlueprintReadWrite)
	UButton* InventoryExit;
	
	UPROPERTY(BlueprintReadWrite)
	UBorder* ItemTrash;
	
	UPROPERTY(BlueprintReadWrite)
	UWidgetAnimation* ExpandActiveInventorySlotAnimation;

	UPROPERTY()
	float MinScaleFor3DZoomOut = 0.25f;

	UPROPERTY(EditAnywhere)
	float Item3DDistFromCamera = 300.f;

	UPROPERTY(EditAnywhere)
	float ScaleToFill3DItemInSlot = 0.8f;
	
private:

	void SuccessfullyPickUpItem(int HoveredOverSlotIndex, FVector2D MouseLocalPosition);
	void SuccessfullyDropItem(int DroppedOnSlotIndex);
	void CancelDraggedItem();
	void ForgetDraggedItem();
	
	void CreateDraggedImage(UImage* DraggedImageBase, FVector2D MouseLocalPosition);
	void CleanUpDraggedImage();

	void Set3DItemPositionToWidget(AItemPickUp3D* Item);
	void Set3DItemRotationToScreen(AItemPickUp3D* Item);
	void Set3DItemScaleToWidget(AItemPickUp3D* Item);

	void Set3DItemPositionToScreenPosition(AItemPickUp3D* Item, FVector2D ScreenPosition);
	FVector2D ScreenToViewportPosition(FVector2D ScreenPosition) const;
	
	UPROPERTY()
	UPlayerInventory* PlayerInventory;
	
	bool bInventoryOpen = false;

	// TODO: Will look into the UMG DragDropOperation, seems to be a cleaner way of doing this
	UPROPERTY()
	UImage* DraggedImage = nullptr;

	UPROPERTY()
	AItemPickUp3D* Dragged3DItem = nullptr;
	
	UPROPERTY()
	UCanvasPanelSlot* DraggedImageSlot = nullptr;
	
	UPROPERTY()
	TArray<AItemPickUp3D*> Menu3DItems;
	
	UPROPERTY()
	TMap<AItemPickUp3D*, F3DItemInventorySlotInfo> InfoFor3DItems;

	FVector2D DraggedImageSize;
};