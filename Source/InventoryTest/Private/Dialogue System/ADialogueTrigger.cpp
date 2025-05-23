// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue System/ADialogueTrigger.h"

#include "InventoryTestPlayerController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AADialogueTrigger::AADialogueTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Item Sprite"));
	RootComponent = InteractTrigger;
	InteractTrigger->SetBoxExtent(FVector::OneVector * 50.0f);

	InteractTrigger->OnComponentBeginOverlap.AddDynamic(this, &AADialogueTrigger::OnOverlapBegin);
	InteractTrigger->OnComponentEndOverlap.AddDynamic(this, &AADialogueTrigger::OnOverlapEnd);
}

void AADialogueTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInventoryTestPlayerController* PlayerController = Cast<AInventoryTestPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (PlayerController->GetCharacter() == OtherActor)
		{
			PlayerController->SetAvailableDialogue(DialogueNodes);
		}
	}
}

void AADialogueTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AInventoryTestPlayerController* PlayerController = Cast<AInventoryTestPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (PlayerController->GetCharacter() == OtherActor)
		{
			PlayerController->RemoveAvailableDialogue();
		}
	}
}

