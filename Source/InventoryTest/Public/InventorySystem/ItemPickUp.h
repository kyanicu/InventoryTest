// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickUp.generated.h"

class AItemPickUp3D;
class UBoxComponent;

// For any info on the item
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ItemName = NAME_None;

	bool bIs3D = false;

	UPROPERTY()
	AItemPickUp3D* ReferenceTo3DItem = nullptr;
};

UCLASS(Abstract)
class INVENTORYTEST_API AItemPickUp : public AActor
{
	GENERATED_BODY()

public:

	AItemPickUp();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FItemData GetItemData() { return ItemData; }

protected:
	UPROPERTY(EditAnywhere)
	FItemData ItemData;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent * InteractTrigger;
};
