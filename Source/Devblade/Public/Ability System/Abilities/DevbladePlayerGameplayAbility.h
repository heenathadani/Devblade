// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Ability System/Abilities/DevbladeGameplayAbility.h"
#include "DevbladePlayerGameplayAbility.generated.h"

class UPlayerUIComponent;
class ADevbladePlayer;
class ADevbladePlayerController;
class UPlayerCombatComponent;

/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladePlayerGameplayAbility : public UDevbladeGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="Devblade Ability")
	ADevbladePlayer* GetPlayerFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Devblade Ability")
	ADevbladePlayerController* GetPlayerControllerFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category="Devblade Ability")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Devblade Ability")
	UPlayerUIComponent* GetPlayerUIComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Devblade Ability")
	FGameplayEffectSpecHandle MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);
	
	UFUNCTION(BlueprintCallable, Category = "Devblade Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag,float& TotalCooldownTime,float& RemainingCooldownTime);

private:
	TWeakObjectPtr<ADevbladePlayer> CachedDevbladePlayer;
	TWeakObjectPtr<ADevbladePlayerController> CachedPlayerController;
};
