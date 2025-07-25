// All rights reserved by Heena Thadani.


#include "Ability System/Abilities/DevbladeEnemyGameplayAbility.h"
#include "Characters/DevbladeEnemy.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"
 #include "DevbladeGameplayTags.h"

ADevbladeEnemy* UDevbladeEnemyGameplayAbility::GetEnemyFromActorInfo() 
{
	if(!CachedEnemy.IsValid())
	{
		CachedEnemy = Cast<ADevbladeEnemy>(CurrentActorInfo->AvatarActor);
	}
	return CachedEnemy.IsValid() ? CachedEnemy.Get() : nullptr;
}

UEnemyCombatComponent* UDevbladeEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo() 
{
	return GetEnemyFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UDevbladeEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	check(EffectClass);
 
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());
 
	FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);
 
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		DevbladeGameplayTags::Shared_SetByCaller_BaseDamage,
		InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
	);
 
	return EffectSpecHandle;
}
