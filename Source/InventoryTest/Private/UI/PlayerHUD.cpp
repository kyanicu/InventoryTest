// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "InventoryTestPlayerController.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "InventorySystem/ItemPickUp3D.h"
#include "InventorySystem/PlayerInventory.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ActiveInventorySlot.h"
#include "UI/InventoryMenu.h"
#include "Widgets/SWindow.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SetInternalBlueprintReferences();

	if (InventoryMenu)
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Hidden);
	}

	if (InventoryExit)
	{
		InventoryExit->SetVisibility(ESlateVisibility::Hidden);
		InventoryExit->OnClicked.AddDynamic(this, &UPlayerHUD::CloseInventory);
	}

	if (ItemTrash)
	{
		ItemTrash->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (ActiveInventorySlot)
	{
		if (UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(ActiveInventorySlot->Slot.Get()))
		{
			DraggedImageSize = CurrentSlot->GetSize();
		}
	}
}

void UPlayerHUD::OpenInventory()
{
	bInventoryOpen = true;
	
	if (InventoryMenu)
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	}

	if (InventoryExit)
	{
		InventoryExit->SetVisibility(ESlateVisibility::Visible);
	}

	if (ItemTrash)
	{
		ItemTrash->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (ExpandActiveInventorySlotAnimation)
	{
		PlayAnimation(ExpandActiveInventorySlotAnimation);
	}
	
	for (AItemPickUp3D* Item : Menu3DItems)
	{
		if (InfoFor3DItems[Item].InventorySlot != -1)
		{
			Item->SetActorHiddenInGame(false);
			Set3DItemPositionToWidget(Item);
			Set3DItemRotationToScreen(Item);
		}
	}

	// TODO: Make cleaner, shouldn't need to call back to PlayerController in a function called by PlayerController
	if (AInventoryTestPlayerController* PlayerController = Cast<AInventoryTestPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PlayerController->SetControlMode(EControlMode::InventoryMenu);
	}
}

void UPlayerHUD::CloseInventory()
{
	if (Dragged3DItem || DraggedImage)
	{
		CancelDraggedItem();
	}
	bInventoryOpen = false;
	
	if (InventoryMenu)
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Hidden);
	}

	if (InventoryExit)
	{
		InventoryExit->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ItemTrash)
	{
		ItemTrash->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (ExpandActiveInventorySlotAnimation)
	{
		PlayAnimation(ExpandActiveInventorySlotAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
	}
	
	for (AItemPickUp3D* Item : Menu3DItems)
	{
		if (InfoFor3DItems[Item].InventorySlot != -1)
		{
			Item->SetActorHiddenInGame(true);
		}
	}

	// Make Cleaner, shouldn't need to call back to PlayerController in a function called by PlayerController
	if (AInventoryTestPlayerController* PlayerController = Cast<AInventoryTestPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PlayerController->SetControlMode(EControlMode::Character);
	}
}

//TODO: Consider using OnMouseButtonDown on each specific ItemImage to handle this instead
FReply UPlayerHUD::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bInventoryOpen && !DraggedImage && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		int SlotHoveredOver = PlayerInventory->GetMouseHoveredInventorySlot(InMouseEvent.GetScreenSpacePosition(), false);
		if (SlotHoveredOver != -2)
		{
			SuccessfullyPickUpItem(SlotHoveredOver, GetCachedGeometry().AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
			
			return FReply::Handled();
		}
	}
	
	return 	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UPlayerHUD::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bInventoryOpen && DraggedImageSlot || Dragged3DItem)
	{
		if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			CancelDraggedItem();
		}
		else
		{
			if (Dragged3DItem)
			{
				Set3DItemPositionToScreenPosition(Dragged3DItem, InMouseEvent.GetScreenSpacePosition());
			}
			else
			{
				DraggedImageSlot->SetPosition(GetCachedGeometry().AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
			}
		}
		
		return FReply::Handled();
	}
		
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UPlayerHUD::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && DraggedImage || Dragged3DItem)
	{
		if (DraggedImage && !PlayerInventory->GetItemData(PlayerInventory->GetDraggedItemIndex()).bIs3D)
		{
			PlayerInventory->GetDraggedItemImage()->SetVisibility(ESlateVisibility::Visible);
		}
		
		int SlotHoveredOver = PlayerInventory->GetMouseHoveredInventorySlot(InMouseEvent.GetScreenSpacePosition(), true);
		if (SlotHoveredOver != -2 && SlotHoveredOver != PlayerInventory->GetDraggedItemIndex())
		{
			SuccessfullyDropItem(SlotHoveredOver);
		}
		else
		{
			// ItemTrash->IsHovered() doesn't work when dragging 2D items as the image widget covers the trash widget, so hovered isn't detected
			const FGeometry TrashGeometry = ItemTrash->GetCachedGeometry();
			const FVector2D MouseLocalPosition = TrashGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

			const FVector2D Size = TrashGeometry.GetLocalSize();
			const bool bHoveredOverTrash =
				MouseLocalPosition.X >= 0.f && MouseLocalPosition.X <= Size.X &&
				MouseLocalPosition.Y >= 0.f && MouseLocalPosition.Y <= Size.Y;

			if (bHoveredOverTrash)
			{
				ForgetDraggedItem();
			}
			else
			{
				CancelDraggedItem();
			}
		}
		
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UPlayerHUD::Track3DItem(AItemPickUp3D* Item, int InventorySlot, UPanelWidget* WidgetToTrack)
{
	Menu3DItems.Add(Item);

	F3DItemInventorySlotInfo Item3DInfo;

	Item3DInfo.WidgetToTrack = WidgetToTrack;
	Item3DInfo.InventorySlot = InventorySlot;
	InfoFor3DItems.Add(Item, Item3DInfo);

	Set3DItemPositionToWidget(Item);
	Set3DItemRotationToScreen(Item);
	Set3DItemScaleToWidget(Item);
}

void UPlayerHUD::Update3DItemTransform(AItemPickUp3D* Item)
{
	if (!Item->IsHidden())
	{
		if (InfoFor3DItems[Item].InventorySlot != PlayerInventory->GetDraggedItemIndex())
		{
			Set3DItemPositionToWidget(Item);
		}

		if (InfoFor3DItems[Item].InventorySlot == -1)
		{
			if (IsAnimationPlaying(ExpandActiveInventorySlotAnimation))
			{
				Set3DItemScaleToWidget(Item);
			}
		}
	
		Set3DItemRotationToScreen(Item);
	}
}

void UPlayerHUD::SuccessfullyPickUpItem(int HoveredOverSlotIndex, FVector2D MouseLocalPosition)
{
	if (!PlayerInventory->GetItemData(HoveredOverSlotIndex).bIs3D)
	{
		UImage* DraggedImageBase = PlayerInventory->GetItemSlotImage(HoveredOverSlotIndex, false);
		CreateDraggedImage(DraggedImageBase, MouseLocalPosition);
		DraggedImageBase->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Dragged3DItem = PlayerInventory->GetItemData(HoveredOverSlotIndex).ReferenceTo3DItem;
	}
	
	PlayerInventory->TrackDraggedItemIndex(HoveredOverSlotIndex);
}

void UPlayerHUD::SuccessfullyDropItem(int DroppedOnSlotIndex)
{
	// Swap item image textures
	UImage* DraggedItemImageOriginal = PlayerInventory->GetDraggedItemImage();
	UImage* ItemToReplaceImage = PlayerInventory->GetItemSlotImage(DroppedOnSlotIndex, Dragged3DItem != nullptr);
	UTexture2D* DraggedItemImageTexture = Cast<UTexture2D>(DraggedItemImageOriginal->GetBrush().GetResourceObject());
	UTexture2D* ImageTextureToReplace = ItemToReplaceImage ? Cast<UTexture2D>(ItemToReplaceImage->GetBrush().GetResourceObject()) : nullptr;
	
	PlayerInventory->SetItemImage(PlayerInventory->GetDraggedItemIndex(), ImageTextureToReplace);
	PlayerInventory->SetItemImage(DroppedOnSlotIndex, DraggedItemImageTexture);

	if (Dragged3DItem)
	{
		InfoFor3DItems[Dragged3DItem].InventorySlot = DroppedOnSlotIndex;
		InfoFor3DItems[Dragged3DItem].WidgetToTrack = ItemToReplaceImage->GetParent();

		Set3DItemScaleToWidget(Dragged3DItem);
		
		Dragged3DItem = nullptr;
	}
	else
	{
		CleanUpDraggedImage();
	}

	if (PlayerInventory->GetItemData(DroppedOnSlotIndex).bIs3D)
	{
		AItemPickUp3D* DroppedOn3DItem = PlayerInventory->GetItemData(DroppedOnSlotIndex).ReferenceTo3DItem;
		
		InfoFor3DItems[DroppedOn3DItem].InventorySlot = PlayerInventory->GetDraggedItemIndex();
		InfoFor3DItems[DroppedOn3DItem].WidgetToTrack = DraggedItemImageOriginal->GetParent();

		Set3DItemScaleToWidget(DroppedOn3DItem);
	}
	
	PlayerInventory->DropDraggedItemIntoSlot(DroppedOnSlotIndex);
}

void UPlayerHUD::CancelDraggedItem()
{
	PlayerInventory->StopTrackingDraggedItemIndex();

	if (Dragged3DItem)
	{
		Dragged3DItem = nullptr;
	}
	else if (DraggedImage)
	{	
		CleanUpDraggedImage();
	}
}

void UPlayerHUD::ForgetDraggedItem()
{
	if (Dragged3DItem)
	{
		Menu3DItems.Remove(Dragged3DItem);
		InfoFor3DItems.Remove(Dragged3DItem);
		Dragged3DItem->Destroy();
		Dragged3DItem = nullptr;
	}
	else
	{
		PlayerInventory->SetItemImage(PlayerInventory->GetDraggedItemIndex(), nullptr);
		CleanUpDraggedImage();
	}

	PlayerInventory->ForgetItem(PlayerInventory->GetDraggedItemIndex());
	PlayerInventory->StopTrackingDraggedItemIndex();
}

void UPlayerHUD::CreateDraggedImage(UImage* DraggedImageBase, FVector2D MouseLocalPosition)
{
	DraggedImage = NewObject<UImage>(this, UImage::StaticClass());
	CanvasPanel->AddChild(DraggedImage);

	if (DraggedImage)
	{
		DraggedImage->SetBrush(DraggedImageBase->GetBrush());
			
		DraggedImageSlot = Cast<UCanvasPanelSlot>(DraggedImage->Slot.Get());
		if (DraggedImageSlot)
		{
			DraggedImageSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			DraggedImageSlot->SetSize(DraggedImageSize);
			DraggedImageSlot->SetPosition(MouseLocalPosition);
		}
	}
}

void UPlayerHUD::CleanUpDraggedImage()
{
	DraggedImage->RemoveFromParent();
	DraggedImage = nullptr;
	DraggedImageSlot = nullptr;
}

void UPlayerHUD::Set3DItemPositionToWidget(AItemPickUp3D* Item)
{
	FVector2D WidgetScreenPosition = InfoFor3DItems[Item].WidgetToTrack->GetCachedGeometry().GetAbsolutePositionAtCoordinates(FVector2D(0.5f,0.5f));

	Set3DItemPositionToScreenPosition(Item, WidgetScreenPosition);
}

void UPlayerHUD::Set3DItemRotationToScreen(AItemPickUp3D* Item)
{
	if (const APlayerCameraManager* CameraManager = GetOwningPlayerCameraManager())
	{
		const FVector ForwardAxis = (Item->GetActorLocation() - CameraManager->GetCameraLocation()).GetSafeNormal();
		const FVector UpAxis = CameraManager->GetActorUpVector();

		Item->SetActorRotation(FRotationMatrix::MakeFromXZ(ForwardAxis, UpAxis).Rotator());
	}
}

void UPlayerHUD::Set3DItemScaleToWidget(AItemPickUp3D* Item)
{
	const F3DItemInventorySlotInfo ItemInfo = InfoFor3DItems[Item];

	if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UMeshComponent * Mesh = Item->GetMeshComponent())
		{
			Item->SetActorScale3D(FVector::OneVector);
			const float ItemExtentRightExtent = Mesh->CalcLocalBounds().BoxExtent.X;

			FVector2D ItemViewportCenter;
			FVector2D ItemViewportRightExtendPoint;
			PlayerController->ProjectWorldLocationToScreen(Item->GetActorLocation(), ItemViewportCenter);
			PlayerController->ProjectWorldLocationToScreen(Item->GetActorLocation() + (PlayerController->PlayerCameraManager->GetActorRightVector() * ItemExtentRightExtent), ItemViewportRightExtendPoint);
			const float ScreenItemExtentRadius = ItemViewportRightExtendPoint.X - ItemViewportCenter.X;
			
			const FVector2D WidgetSize = ItemInfo.WidgetToTrack->GetCachedGeometry().GetAbsoluteSize() * ScaleToFill3DItemInSlot; // Scale for offset so it isn't right at slot borders
			
			Item->SetActorScale3D(FVector((WidgetSize.X * 0.5f) / ScreenItemExtentRadius));
		}
	}
}

void UPlayerHUD::Set3DItemPositionToScreenPosition(AItemPickUp3D* Item, FVector2D ScreenPosition)
{
	if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		const FVector2D ViewportPosition = ScreenToViewportPosition(ScreenPosition);
		
		FVector WorldLocation, WorldDirection;
		PlayerController->DeprojectScreenPositionToWorld(ViewportPosition.X, ViewportPosition.Y, WorldLocation, WorldDirection);
		
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		
		const FVector TargetLocation = CameraLocation + (WorldDirection * Item3DDistFromCamera);

		// Move or spawn the actor
		Item->SetActorLocation(TargetLocation);
	}
}

// Will need to update to consider for DPI scaling and viewport origin offset
FVector2D UPlayerHUD::ScreenToViewportPosition(FVector2D ScreenPosition) const
{
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(PlayerInventory, ScreenPosition, PixelPosition, ViewportPosition);

	return PixelPosition;
}