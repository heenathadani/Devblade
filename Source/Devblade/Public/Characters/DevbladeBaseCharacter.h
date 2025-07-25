// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "DevbladeBaseCharacter.generated.h"

class UDevbladeAttributeSet;
class UDevbladeAbilitySystemComponent;
class UDataAsset_StartUpDataBase;
class UMotionWarpingComponent;

UCLASS()
class DEVBLADE_API ADevbladeBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADevbladeBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface.

	//~ Begin IPawnCombatComponent Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatComponent Interface.

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponentBase* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface.
	
protected:
	
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	UDevbladeAbilitySystemComponent* DevbladeAbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	UDevbladeAttributeSet* DevbladeAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;
	
public:
	FORCEINLINE UDevbladeAbilitySystemComponent* GetDevbladeAbilitySystemComponent() const { return DevbladeAbilitySystemComponent; }
	FORCEINLINE UDevbladeAttributeSet* GetDevbladeAttributeSet() const { return DevbladeAttributeSet; }
};
