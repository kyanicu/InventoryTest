// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTestPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Dialogue System/DialogueManager.h"
#include "InventorySystem/PlayerInventory.h"
#include "UI/PlayerHUD.h"

AInventoryTestPlayerController::AInventoryTestPlayerController()
{
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AInventoryTestPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD = CreateWidget<UPlayerHUD>(this, HUDWidgetClass);
	
	if (HUD)
	{
		HUD->AddToViewport();
	}

	Inventory = NewObject<UPlayerInventory>(this, FName("PlayerInventory"));
	Inventory->Initialize(HUD);

	DialogueManager = NewObject<UDialogueManager>(this, FName("DialogueManager"));
	if (DialogueManager)
	{
		DialogueManager->OnDialogueEnded.BindUObject(this, &AInventoryTestPlayerController::HandleDialogueEnd);
	}
		
	HUD->SetPlayerInventory(Inventory);

	// Start player on the inventory
	AttemptOpenInventory();
}

void AInventoryTestPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		// Set up Inventory management bindings
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &AInventoryTestPlayerController::AttemptOpenInventory);
		EnhancedInputComponent->BindAction(CloseInventoryAction, ETriggerEvent::Triggered, this, &AInventoryTestPlayerController::AttemptCloseInventory);
		EnhancedInputComponent->BindAction(DialogueAction, ETriggerEvent::Triggered, this, &AInventoryTestPlayerController::AttemptHandleDialogue);
	}
}

void AInventoryTestPlayerController::SetControlMode(EControlMode ControlMode)
{
	CurrentControlMode = ControlMode;

	SetShowMouseCursor(CurrentControlMode == EControlMode::InventoryMenu);
}

void AInventoryTestPlayerController::SetAvailableDialogue(TArray<FDialogueNodeInfo> DialogueInfo)
{
	AvailableDialogue = DialogueInfo;
}

void AInventoryTestPlayerController::RemoveAvailableDialogue()
{
	AvailableDialogue.Empty();
}

void AInventoryTestPlayerController::AttemptOpenInventory()
{
	if (HUD && !HUD->GetIsInventoryOpen() && CurrentControlMode == EControlMode::Character)
	{
		HUD->OpenInventory();
	}
}

void AInventoryTestPlayerController::AttemptCloseInventory()
{
	if (HUD && HUD->GetIsInventoryOpen())
	{
		HUD->CloseInventory();
	}
}

void AInventoryTestPlayerController::AttemptHandleDialogue()
{
	if (CurrentControlMode == EControlMode::Character && !AvailableDialogue.IsEmpty())
	{
		SetControlMode(EControlMode::Dialogue);

		DialogueManager->StartDialogue(AvailableDialogue);
	}
	else if (CurrentControlMode == EControlMode::Dialogue)
	{
		DialogueManager->AdvanceDialogue();
	}
}

void AInventoryTestPlayerController::HandleDialogueEnd()
{
	SetControlMode(EControlMode::Character);
}
