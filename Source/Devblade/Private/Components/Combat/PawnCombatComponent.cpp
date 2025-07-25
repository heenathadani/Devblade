// All rights reserved by Heena Thadani.


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/BaseWeapon.h"
#include "Components/BoxComponent.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ABaseWeapon* InWeaponToRegister,
	bool bRegisteredAsEquippedWeapon)
{
	checkf(!CharacterWeaponMap.Contains(InWeaponTagToRegister), TEXT("Weapon with tag %s already exists in the map"), *InWeaponTagToRegister.ToString());
	check(InWeaponTagToRegister.IsValid());

	CharacterWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);
	
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
	
	if(bRegisteredAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister ;
	}
}

ABaseWeapon* UPawnCombatComponent::GetCharacterWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if(CharacterWeaponMap.Contains(InWeaponTagToGet))
	{
		if(ABaseWeapon* const* FoundWeapon = CharacterWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

ABaseWeapon* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if(!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}
	return GetCharacterWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if(ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	else
	{
		ToggleBodyCollsionBoxCollision(bShouldEnable,ToggleDamageType);
	}
	//TODO: Handle Body parts collision
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractActor)
{
}

void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	ABaseWeapon* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
 
	check(WeaponToToggle);
 
	if (bShouldEnable)
	{
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 
		OverlappedActors.Empty();
	}
}

void UPawnCombatComponent::ToggleBodyCollsionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
}
