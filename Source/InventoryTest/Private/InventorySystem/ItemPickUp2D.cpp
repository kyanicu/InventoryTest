// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemPickUp2D.h"

#include "Components/BillboardComponent.h"


// Sets default values
AItemPickUp2D::AItemPickUp2D()
{
	// Setup item visual sprite
	ItemSprite = CreateDefaultSubobject<UBillboardComponent>(MakeUniqueObjectName(this, UBillboardComponent::StaticClass(), FName("Item Sprite")));
	ItemSprite->SetupAttachment(RootComponent);
	ItemSprite->bHiddenInGame = false;
}

UTexture2D* AItemPickUp2D::GetItemImage() const
{
	return ItemSprite->Sprite.Get();
}


