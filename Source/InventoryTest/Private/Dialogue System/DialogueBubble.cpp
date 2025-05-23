// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue System/DialogueBubble.h"

#include "Components/TextBlock.h"

void UDialogueBubble::NativeConstruct()
{
	Super::NativeConstruct();

	SetInternalBlueprintReferences();
}

void UDialogueBubble::SetText(FText NewText)
{
	if (TextBlock)
	{
		TextBlock->SetText(NewText);
	}
}
