// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Dialogue System/ADialogueTrigger.h"

#include "InventoryTestPlayerController.generated.h"

class UPlayerHUD;
class AItemPickUp;
class UPlayerInventory;
class UInputAction;
class UDialogueManager;

UENUM()
enum class EControlMode { Character, InventoryMenu, Dialogue };

UCLASS()
class INVENTORYTEST_API AInventoryTestPlayerController : public APlayerController
{
	GENERATED_BODY()

	AInventoryTestPlayerController();

public:

	void BeginPlay() override;

	void SetupInputComponent() override;

	UPlayerHUD* GetPlayerHUD() const { return HUD; } 
	UPlayerInventory* GetInventory() { return Inventory; }
	UDialogueManager* GetDialogue() { return DialogueManager; }

	EControlMode GetControlMode() const { return CurrentControlMode; }

	void SetControlMode(EControlMode ControlMode);

	void SetAvailableDialogue(TArray<FDialogueNodeInfo> DialogueInfo);
	void RemoveAvailableDialogue();
	
protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUD> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CloseInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DialogueAction;

private :
	
	UFUNCTION()
	void AttemptOpenInventory();

	UFUNCTION()
	void AttemptCloseInventory();

	UFUNCTION()
	void AttemptHandleDialogue();

	UFUNCTION()
	void HandleDialogueEnd();

	UPROPERTY()
	UPlayerHUD* HUD;
	
	UPROPERTY()
	UPlayerInventory* Inventory;

	UPROPERTY()
	UDialogueManager* DialogueManager;

	EControlMode CurrentControlMode;

	TArray<FDialogueNodeInfo> AvailableDialogue;
};
