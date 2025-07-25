// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PlayerTypes/DevbladeStructTypes.h"
#include "DevbladeAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category="Devblade Ability", meta = (ApplyLevel = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FDevbladePlayerAbilitySet>& InDefaultWeaponAbilities,const TArray<FDevbladePlayerSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
 

	UFUNCTION(BlueprintCallable, Category="Devblade Ability")
	void RemoveGrantedPlayerWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category="Devblade Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
