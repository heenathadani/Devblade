// All rights reserved by Heena Thadani.


#include "Ability System/Abilities/DevbladePlayerGameplayAbility.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"
#include "Controllers//DevbladePlayerController.h"
#include "Characters/DevbladePlayer.h"
#include "DevbladeGameplayTags.h"

ADevbladePlayer* UDevbladePlayerGameplayAbility::GetPlayerFromActorInfo()
{
	if(!CachedDevbladePlayer.IsValid())
	{
		CachedDevbladePlayer = Cast<ADevbladePlayer>(CurrentActorInfo->AvatarActor);
	}
	return CachedDevbladePlayer.IsValid()? CachedDevbladePlayer.Get(): nullptr;
}

ADevbladePlayerController* UDevbladePlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if(!CachedPlayerController.IsValid())
	{
		CachedPlayerController = Cast<ADevbladePlayerController>(CurrentActorInfo->PlayerController);
	}
	return CachedPlayerController.IsValid()? CachedPlayerController.Get(): nullptr;
}

UPlayerCombatComponent* UDevbladePlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerFromActorInfo()->GetPlayerCombatComponent();
}

UPlayerUIComponent* UDevbladePlayerGameplayAbility::GetPlayerUIComponentFromActorInfo()
{
	return GetPlayerFromActorInfo()->GetPlayerUIComponent();
}

FGameplayEffectSpecHandle UDevbladePlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag,
	int32 InUsedComboCount)
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
		InWeaponBaseDamage);

	if(InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	}
	return EffectSpecHandle;
}

bool UDevbladePlayerGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag,
	float& TotalCooldownTime, float& RemainingCooldownTime)
{
	check(InCooldownTag.IsValid());
 
	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
 
	TArray< TPair <float,float> > TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);
 
	if (!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
		TotalCooldownTime = TimeRemainingAndDuration[0].Value;
	}
 
	return RemainingCooldownTime > 0.f;
}
