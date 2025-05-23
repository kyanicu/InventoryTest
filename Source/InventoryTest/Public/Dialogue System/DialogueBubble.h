// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueBubble.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORYTEST_API UDialogueBubble : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	void SetText(FText NewText);
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetInternalBlueprintReferences();

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* TextBlock;
};
