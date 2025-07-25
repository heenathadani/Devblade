// All rights reserved by Heena Thadani.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
 #include "DevbladeGameplayTags.h"
#include "DevbladeFunctionLibrary.h"
#include "Characters/DevbladeEnemy.h"
#include "Components/BoxComponent.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	OverlappedActors.AddUnique(HitActor);
 
	//TODO:: Implement block check
	bool bIsValidBlock = false;
 
	const bool bIsPlayerBlocking = UDevbladeFunctionLibrary::NativeDoesActorHaveTag(HitActor,DevbladeGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = UDevbladeFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(),DevbladeGameplayTags::Enemy_Status_Unbloackable);
 
	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		//TODO::check if the block is valid
		bIsValidBlock = UDevbladeFunctionLibrary::IsValidBlock(GetOwningPawn(),HitActor);
	}
 
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
 
	if (bIsValidBlock)
	{
		//TODO::Handle successful block
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			 HitActor,
			 DevbladeGameplayTags::Player_Event_SuccessfulBlock,
			 EventData
		 );
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			DevbladeGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}

void UEnemyCombatComponent::ToggleBodyCollsionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	ADevbladeEnemy* OwningEnemyCharacter = GetOwningPawn<ADevbladeEnemy>();
 
	check(OwningEnemyCharacter);
 
	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();
 
	check(LeftHandCollisionBox && RightHandCollisionBox);
 
	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
 
	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
 
	default:
		break;
	}
 
	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
