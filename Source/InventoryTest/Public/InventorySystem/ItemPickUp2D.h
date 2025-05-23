// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemPickUp.h"
#include "ItemPickUp2D.generated.h"

class UTexture2D;
class UBillboardComponent;
UCLASS()
class INVENTORYTEST_API AItemPickUp2D : public AItemPickUp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UBillboardComponent* ItemSprite;
	
public:
	
	AItemPickUp2D();

	UTexture2D* GetItemImage() const;
};
