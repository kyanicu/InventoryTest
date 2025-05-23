// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/InventoryTestGameMode.h"
#include "InventoryTestCharacter.h"
#include "InventoryTestPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AInventoryTestGameMode::AInventoryTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_InventoryTestPlayerController"));

	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
