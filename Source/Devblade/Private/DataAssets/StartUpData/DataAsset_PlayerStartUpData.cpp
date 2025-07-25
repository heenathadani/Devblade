// All rights reserved by Heena Thadani.


#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"
#include "Ability System/Abilities/DevbladePlayerGameplayAbility.h"

void UDataAsset_PlayerStartUpData::GiveToAbilitySystemComponent(UDevbladeAbilitySystemComponent* InASCToGive,
                                                                int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	for(FDevbladePlayerAbilitySet& AbilitySet : PlayerStartupAbilitySets)
	{
		if(!AbilitySet.IsValid())
		{
			continue;
		}
		FGameplayAbilitySpec AbilitySpec((AbilitySet.AbilityToGrant.Get()));
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
