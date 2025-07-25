// All rights reserved by Heena Thadani.


#include "PlayerTypes/DevbladeStructTypes.h"
#include "Ability System/Abilities/DevbladePlayerGameplayAbility.h"

bool FDevbladePlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
