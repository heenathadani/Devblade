// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DevbladeBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "DevbladePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
class UPlayerCombatComponent;
struct FInputActionValue;
class UPlayerUIComponent;
/**
 * 
 */
UCLASS()
class DEVBLADE_API ADevbladePlayer : public ADevbladeBaseCharacter
{
	GENERATED_BODY()
	
	public:
	
		ADevbladePlayer();
	
	//~ Begin IPawnCombatComponent Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatComponent Interface.

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponentBase* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	//~ End IPawnUIInterface Interface.

	protected:
		//~ Begin APawn Interface.
		virtual void PossessedBy(AController* NewController) override;
		//~ End APawn Interface.
	
		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
		virtual void BeginPlay() override;

	private:
		#pragma region Components
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		UPlayerCombatComponent* PlayerCombatComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		UPlayerUIComponent* PlayerUIComponent;
	#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;
	
	UPROPERTY()
	FVector2D SwitchDirection = FVector2D::ZeroVector;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);

	void Input_PickUpStonesStarted(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
#pragma endregion

public:
	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent;}
	
};
 
