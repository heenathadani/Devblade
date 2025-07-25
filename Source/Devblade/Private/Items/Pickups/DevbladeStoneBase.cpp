// All rights reserved by Heena Thadani.


#include "Items/Pickups/DevbladeStoneBase.h"
 #include "Ability System/DevbladeAbilitySystemComponent.h"
 #include "DevbladeGameplayTags.h"
#include "Characters/DevbladePlayer.h"

void ADevbladeStoneBase::Consume(UDevbladeAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	check(StoneGameplayEffectClass);
 
	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
 	
	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);
 
	BP_OnStoneConsumed();
}

void ADevbladeStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                             AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                             const FHitResult& SweepResult)
{
	if (ADevbladePlayer* OverlappedPlayerCharacter = Cast<ADevbladePlayer>(OtherActor))
	{
		OverlappedPlayerCharacter->GetDevbladeAbilitySystemComponent()->TryActivateAbilityByTag(DevbladeGameplayTags::Player_Ability_PickUp_Stones);
	}
}
