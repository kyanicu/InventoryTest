// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADialogueTrigger.generated.h"

class AInventoryTestCharacter;
USTRUCT(BlueprintType)
struct FDialogueNodeInfo
{
	GENERATED_BODY()

	/** The Character to speak (none will set to player character) */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AInventoryTestCharacter> Speaker;

	UPROPERTY(EditAnywhere)
	FText Text; // In a larger project I'd use the string table to handle localization properly
};

// In a larger project I would use a dialogue plugin , and reference characters by their tag
// For simplicity in this, I'm doing a rather simple system that doesn't support branching dialogue
class UBoxComponent;
UCLASS()
class INVENTORYTEST_API AADialogueTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AADialogueTrigger();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	
	/** The Dialogue info */
	UPROPERTY(EditAnywhere)
	TArray<FDialogueNodeInfo> DialogueNodes;
	
public:
	
	UPROPERTY(EditAnywhere)
	UBoxComponent * InteractTrigger;
};