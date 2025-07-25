// All rights reserved by Heena Thadani.


#include "Ability System/Abilities/PlayerGA_PickUpStones.h"
#include "Kismet/KismetSystemLibrary.h"
 #include "Characters/DevbladePlayer.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Items/PickUps/DevbladeStoneBase.h"

void UPlayerGA_PickUpStones::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	GetPlayerUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPlayerGA_PickUpStones::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	GetPlayerUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGA_PickUpStones::CollectStones()
{
	CollectedStones.Empty();
 
	TArray<FHitResult> TraceHits;
 
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerFromActorInfo(),
		GetPlayerFromActorInfo()->GetActorLocation(),
		GetPlayerFromActorInfo()->GetActorLocation() + -GetPlayerFromActorInfo()->GetActorUpVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		(-GetPlayerFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),
		StoneTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebugShape? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		TraceHits,
		true
	);
 
	for (const FHitResult& TraceHit : TraceHits)
	{
		if (ADevbladeStoneBase* FoundStone = Cast<ADevbladeStoneBase>(TraceHit.GetActor()))
		{
			CollectedStones.AddUnique(FoundStone);
		}
	}
 
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UPlayerGA_PickUpStones::ConsumeStones()
{
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}
 
	for (ADevbladeStoneBase* CollectedStone : CollectedStones)
	{
		if (CollectedStone)
		{
			CollectedStone->Consume(GetPlayerAbilitySystemComponentFromActorInfo(),GetAbilityLevel());
		}
	}
}
