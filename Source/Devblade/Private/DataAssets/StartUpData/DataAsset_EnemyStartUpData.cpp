// All rights reserved by Heena Thadani.


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "Ability System/DevbladeAbilitySystemComponent.h"
#include "Ability System/Abilities/DevbladeEnemyGameplayAbility.h"


void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UDevbladeAbilitySystemComponent* InASCToGive,
                                                               int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	if(!EnemyCombatAbilities.IsEmpty())
	{
		for(const TSubclassOf<UDevbladeEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			if(!AbilityClass)
			{
				continue;
			}

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
