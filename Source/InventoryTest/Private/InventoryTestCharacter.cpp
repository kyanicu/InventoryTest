// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryTestCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InventoryTestPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Dialogue System/DialogueBubble.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AInventoryTestCharacter

AInventoryTestCharacter::AInventoryTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create WidgetComponent for dialogue and set above character head
	DialogueWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	DialogueWidgetComponent->SetupAttachment(RootComponent);
	DialogueWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // or World
	DialogueWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) + DialogueBubbleOffset);
	DialogueWidgetComponent->SetHiddenInGame(false);
	DialogueWidgetComponent->SetPivot(FVector2D(0.5f, 1.0f));
	DialogueWidgetComponent->SetDrawAtDesiredSize(true);
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AInventoryTestCharacter::ShowDialogueBubble(FText BubbleText)
{
	if (DialogueBubbleWidget)
	{
		DialogueBubbleWidget->SetText(BubbleText);
		DialogueWidgetComponent->SetHiddenInGame(false);
	}
}

void AInventoryTestCharacter::HideDialogueBubble()
{
	if (DialogueBubbleWidget)
	{
		DialogueWidgetComponent->SetHiddenInGame(true);
		DialogueBubbleWidget->SetText(FText::FromString(""));
	}
}

void AInventoryTestCharacter::Jump()
{
	if (IsValid(InventoryTestPlayerController) && InventoryTestPlayerController->GetControlMode() == EControlMode::Character)
	{
		Super::Jump();
	}
}

void AInventoryTestCharacter::StopJumping()
{
	if (IsValid(InventoryTestPlayerController) && InventoryTestPlayerController->GetControlMode() == EControlMode::Character)
	{
		Super::StopJumping();
	}
}

void AInventoryTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	InventoryTestPlayerController = Cast<AInventoryTestPlayerController>(Controller);

	if (DialogueBubbleClass)
	{
		DialogueWidgetComponent->SetWidgetClass(DialogueBubbleClass);
		DialogueWidgetComponent->InitWidget();
		DialogueBubbleWidget = Cast<UDialogueBubble>(DialogueWidgetComponent->GetWidget());

		// Ensure the widget is initialized properly to ensure text gets updated correctly for first change
		DialogueWidgetComponent->SetHiddenInGame(false);
		DialogueBubbleWidget->InvalidateLayoutAndVolatility();
		DialogueBubbleWidget->ForceLayoutPrepass();
		DialogueWidgetComponent->UpdateWidget();

		DialogueWidgetComponent->SetHiddenInGame(true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInventoryTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AInventoryTestCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AInventoryTestCharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInventoryTestCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInventoryTestCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AInventoryTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (IsValid(InventoryTestPlayerController) && InventoryTestPlayerController->GetControlMode() == EControlMode::Character)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInventoryTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (IsValid(InventoryTestPlayerController) && InventoryTestPlayerController->GetControlMode() == EControlMode::Character || InventoryTestPlayerController->GetControlMode() == EControlMode::Dialogue)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}