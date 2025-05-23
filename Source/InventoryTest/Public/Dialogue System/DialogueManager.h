// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dialogue System/ADialogueTrigger.h"

#include "DialogueManager.generated.h"

class AInventoryTestCharacter;
/**
 * 
 */
UCLASS()
class INVENTORYTEST_API UDialogueManager : public UObject
{
	GENERATED_BODY()

public:

	void StartDialogue(TArray<FDialogueNodeInfo> DialogueInfo);
	void AdvanceDialogue();
	void EndDialogue();

	DECLARE_DELEGATE(FOnDialogueEnded)
	FOnDialogueEnded OnDialogueEnded;
	
private:

	void DisplayCurrentBubble();
	void HideCurrentBubble();

	AInventoryTestCharacter* CharacterGetCurrentCharacter();
	
	TArray<FDialogueNodeInfo> CurrentDialogue;
	int CurrentNode = 0;
};
