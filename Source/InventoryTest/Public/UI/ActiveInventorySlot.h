// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActiveInventorySlot.generated.h"

class UImage;
class UTexture2D;
/**
 * 
 */
UCLASS()
class INVENTORYTEST_API UActiveInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	UImage* GetItemImage() const { return ItemImage; }
	void SetItemImage(UTexture2D* NewItemImage);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetInternalBlueprintReferences();
	
	UPROPERTY(BlueprintReadWrite)
	UImage* ItemImage;

};
