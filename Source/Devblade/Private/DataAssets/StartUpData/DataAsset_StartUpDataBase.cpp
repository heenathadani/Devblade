// All rights reserved by Heena Thadani.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Ability System/Abilities/DevbladeGameplayAbility.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UDevbladeAbilitySystemComponent* InASCToGive,
                                                              int32 ApplyLevel)
{
	check(InASCToGive);
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	if(!StartupGameplayEffects.IsEmpty())
	{
		for(const TSubclassOf<UGameplayEffect>& EffectClass : StartupGameplayEffects)
		{
			if(!EffectClass) continue;

			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			InASCToGive->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, InASCToGive->MakeEffectContext());
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UDevbladeGameplayAbility>>& InAbilitiesToGive,
	UDevbladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if(InAbilitiesToGive.IsEmpty())
	{
		return;
	}
	for(const TSubclassOf<UDevbladeGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if(!Ability)
		{
			continue;
		}
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
