// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemPickUp3D.h"

#include "InventoryTestPlayerController.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PlayerHUD.h"


// Sets default values
AItemPickUp3D::AItemPickUp3D()
{
	ItemData.bIs3D = true;
	ItemData.ReferenceTo3DItem = this;
}

void AItemPickUp3D::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent = GetComponentByClass<UMeshComponent>();
	if (IsValid(MeshComponent))
	{
		MeshComponent->SetGenerateOverlapEvents(false);
	}

	if (const AInventoryTestPlayerController* PlayerController = Cast<AInventoryTestPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PlayerHUD = PlayerController->GetPlayerHUD();
	}
}

void AItemPickUp3D::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsInventoryItem && PlayerHUD)
	{
		// Update in world time, not UI time, otherwise there would be awful lag
		PlayerHUD->Update3DItemTransform(this);
	}
}


void AItemPickUp3D::InitializeAsMenuItem(int InventorySlot, UPanelWidget* WidgetToTrack, bool bIsHidden)
{
	InteractTrigger->SetActive(false);

	if (MeshComponent)
	{
		// Set to render above all other objects
		MeshComponent->SetRenderCustomDepth(true);
		MeshComponent->SetCustomDepthStencilValue(255);
		MeshComponent->bRenderInMainPass = true;
		MeshComponent->SetTranslucentSortPriority(999);
	}
	
	SetActorHiddenInGame(bIsHidden);

	if (const AInventoryTestPlayerController* PlayerController = Cast<AInventoryTestPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PlayerHUD = PlayerController->GetPlayerHUD();

		if (PlayerHUD)
		{
			PlayerHUD->Track3DItem(this, InventorySlot, WidgetToTrack);
			bIsInventoryItem = true;
		}

		AttachToActor(PlayerController->PlayerCameraManager, FAttachmentTransformRules::KeepWorldTransform);
	}
}
