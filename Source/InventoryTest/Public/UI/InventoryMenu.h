// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class INVENTORYTEST_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetItemImage(int ItemSlotIndex, UTexture2D* NewItemImage);
	TArray<UImage*> GetItemImages() { return ItemImages; }
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetInternalBlueprintReferences();

	UPROPERTY(BlueprintReadWrite)
	TArray<UImage*> ItemImages;

};
