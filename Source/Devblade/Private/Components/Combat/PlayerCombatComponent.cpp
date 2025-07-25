// All rights reserved by Heena Thadani.


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Weapons/PlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DevbladeGameplayTags.h"

#include "DevbladeDebugHelper.h"

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if(OverlappedActors.Contains(HitActor))
		return;
	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), DevbladeGameplayTags::Shared_Event_MeleeHit, Data);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), DevbladeGameplayTags::Player_Event_HitPause, FGameplayEventData());
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), DevbladeGameplayTags::Player_Event_HitPause, FGameplayEventData());
}

APlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<APlayerWeapon>(GetCharacterWeaponByTag(InWeaponTag));
}

APlayerWeapon* UPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	return Cast<APlayerWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UPlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
	return GetPlayerCurrentEquippedWeapon()->PlayerWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}
