// All rights reserved by Heena Thadani.

#include "Characters/DevbladePlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/DevbladeInputComponent.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"
#include "DevbladeGameplayTags.h"
#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameModes/DevbladeBaseGameMode.h"

ADevbladePlayer::ADevbladePlayer()
{
	// Initialize the capsule component size
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Disable rotation of the character with the controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create and configure the camera boom (spring arm)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 65.0f);
	CameraBoom->bUsePawnControlRotation = true;

	// Define the follow camera properties
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Configuration of character movement properties
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
}

UPawnCombatComponent* ADevbladePlayer::GetPawnCombatComponent() const
{
	return PlayerCombatComponent;
}

UPawnUIComponentBase* ADevbladePlayer::GetPawnUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* ADevbladePlayer::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

void ADevbladePlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(!CharacterStartUpData.IsNull())
	{
		if(UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{	
			int32 AbilityApplyLevel = 1;
 
			if (ADevbladeBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<ADevbladeBaseGameMode>())
			{
				switch (BaseGameMode->GetCurrentGameDifficulty())
				{
				case EDevbladeGameDifficulty::Easy:
					AbilityApplyLevel = 4;
					break;
 
				case EDevbladeGameDifficulty::Normal:
					AbilityApplyLevel = 3;
					break;
 
				case EDevbladeGameDifficulty::Hard:
					AbilityApplyLevel = 2;
					break;
 
				case EDevbladeGameDifficulty::VeryHard:
					AbilityApplyLevel = 1;
					break;
 
				default:
					break;
				}
			}
 			
			LoadedData->GiveToAbilitySystemComponent(DevbladeAbilitySystemComponent,AbilityApplyLevel);
		}
	}
}

void ADevbladePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid InputConfigDataAsset."));
	ULocalPlayer* LocalPlayer= GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);

	UDevbladeInputComponent* DevbladeInputComponent = CastChecked<UDevbladeInputComponent>(PlayerInputComponent);
	DevbladeInputComponent->BindNativeAction(InputConfigDataAsset, DevbladeGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	DevbladeInputComponent->BindNativeAction(InputConfigDataAsset, DevbladeGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	DevbladeInputComponent->BindNativeAction(InputConfigDataAsset,DevbladeGameplayTags::InputTag_SwitchTarget,ETriggerEvent::Triggered,this,&ThisClass::Input_SwitchTargetTriggered);
	DevbladeInputComponent->BindNativeAction(InputConfigDataAsset,DevbladeGameplayTags::InputTag_SwitchTarget,ETriggerEvent::Completed,this,&ThisClass::Input_SwitchTargetCompleted);

	DevbladeInputComponent->BindNativeAction(InputConfigDataAsset,DevbladeGameplayTags::InputTag_PickUp_Stones,ETriggerEvent::Started,this,&ThisClass::Input_PickUpStonesStarted);
	
	DevbladeInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void ADevbladePlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ADevbladePlayer::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	if(MovementVector.Y != 0.0f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if(MovementVector.X != 0.0f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADevbladePlayer::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookVector = InputActionValue.Get<FVector2D>();
	if(LookVector.X != 0.0f || LookVector.Y != 0.0f)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ADevbladePlayer::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void ADevbladePlayer::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
 
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X>0.f? DevbladeGameplayTags::Player_Event_SwitchTarget_Right : DevbladeGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);
}

void ADevbladePlayer::Input_PickUpStonesStarted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
 
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		DevbladeGameplayTags::Player_Event_ConsumeStones,
		Data
	);
}

void ADevbladePlayer::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	DevbladeAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void ADevbladePlayer::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	DevbladeAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
