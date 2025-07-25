// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Ability System/Abilities/DevbladeGameplayAbility.h"
#include "DevbladeEnemyGameplayAbility.generated.h"
class UEnemyCombatComponent;
class ADevbladeEnemy;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladeEnemyGameplayAbility : public UDevbladeGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Devblade Ability")
	ADevbladeEnemy* GetEnemyFromActorInfo() ;
	
	UFUNCTION(BlueprintPure, Category = "Devblade Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo() ;

	UFUNCTION(BlueprintPure, Category = "Devblade Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,const FScalableFloat& InDamageScalableFloat);

private:
	TWeakObjectPtr<ADevbladeEnemy> CachedEnemy;
};
