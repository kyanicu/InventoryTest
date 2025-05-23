// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue System/DialogueManager.h"

#include "InventoryTestCharacter.h"
#include "InventoryTestPlayerController.h"
#include "Dialogue System/ADialogueTrigger.h"
#include "Kismet/GameplayStatics.h"

void UDialogueManager::StartDialogue(TArray<FDialogueNodeInfo> DialogueInfo)
{
	if (!DialogueInfo.IsEmpty())
	{
		CurrentDialogue = DialogueInfo;

		DisplayCurrentBubble();
	}
}

void UDialogueManager::AdvanceDialogue()
{
	HideCurrentBubble();
	
	CurrentNode++;
	
	if (CurrentNode < CurrentDialogue.Num())
	{
		DisplayCurrentBubble();
	}
	else
	{
		EndDialogue();
	}
}

void UDialogueManager::EndDialogue()
{
	CurrentDialogue.Empty();
	CurrentNode = 0;

	OnDialogueEnded.Execute();
}

void UDialogueManager::DisplayCurrentBubble()
{
	CharacterGetCurrentCharacter()->ShowDialogueBubble(CurrentDialogue[CurrentNode].Text);
}

void UDialogueManager::HideCurrentBubble()
{
	CharacterGetCurrentCharacter()->HideDialogueBubble();
}

AInventoryTestCharacter* UDialogueManager::CharacterGetCurrentCharacter()
{
	AInventoryTestCharacter* CurrentSpeaker = CurrentDialogue[CurrentNode].Speaker.Get();
	return IsValid(CurrentSpeaker) ? CurrentSpeaker : Cast<AInventoryTestCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}
