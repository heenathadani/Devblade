// All rights reserved by Heena Thadani.


#include "Items/Weapons/PlayerWeapon.h"

void APlayerWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

const TArray<FGameplayAbilitySpecHandle>& APlayerWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
