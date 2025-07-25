// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UGameplayEffect;
class UDevbladeAbilitySystemComponent;
class UDevbladeGameplayAbility;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UDevbladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<UDevbladeGameplayAbility>> ActivateOnGivenAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<UDevbladeGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartupGameplayEffects;
	
	void GrantAbilities(const TArray<TSubclassOf<UDevbladeGameplayAbility>>& InAbilitiesToGive, UDevbladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
	
};
