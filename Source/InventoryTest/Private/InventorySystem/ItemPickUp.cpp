// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemPickUp.h"

#include "InventoryTestCharacter.h"
#include "InventoryTestPlayerController.h"
#include "Components/BoxComponent.h"
#include "InventorySystem/PlayerInventory.h"

AItemPickUp::AItemPickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Item Sprite"));
	RootComponent = InteractTrigger;
	InteractTrigger->SetBoxExtent(FVector::OneVector * 50.0f);

	InteractTrigger->OnComponentBeginOverlap.AddDynamic(this, &AItemPickUp::OnOverlapBegin);

	ItemData.ItemName = FName("DefaultName");
}

void AItemPickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInventoryTestCharacter* Character = Cast<AInventoryTestCharacter>(OtherActor))
	{
		if (AInventoryTestPlayerController* PlayerController = Cast<AInventoryTestPlayerController>(Character->GetController()))
		{
			if (UPlayerInventory* PlayerInventory = PlayerController->GetInventory())
			{
				PlayerInventory->PickUpItem(this);
			}
		}
	}
}