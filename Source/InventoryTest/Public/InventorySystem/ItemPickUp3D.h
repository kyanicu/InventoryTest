// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemPickUp.h"
#include "ItemPickUp3D.generated.h"

class UPlayerHUD;
class UPanelWidget;
UCLASS()
class INVENTORYTEST_API AItemPickUp3D : public AItemPickUp
{
	GENERATED_BODY()

public:
	AItemPickUp3D();

	void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	void InitializeAsMenuItem(int InventorySlot, UPanelWidget* WidgetToTrack, bool bIsHidden);

	bool GetIsInventoryItem() { return bIsInventoryItem; };

	UMeshComponent* GetMeshComponent() const { return MeshComponent; }
	
private:

	UPROPERTY()
	UMeshComponent* MeshComponent;

	bool bIsInventoryItem = false;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;
};
