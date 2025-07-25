// All rights reserved by Heena Thadani.


#include "Ability System/Abilities/DevbladeGameplayAbility.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DevbladeFunctionLibrary.h"
#include "DevbladeGameplayTags.h"

void UDevbladeGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if(AbilityActivationPolicy == EDevbladeAbilityActivationPolicy::OnGiven)
	{
		if(ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, true);
		}
	}
}

void UDevbladeGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if(AbilityActivationPolicy == EDevbladeAbilityActivationPolicy::OnGiven)
	{
		if(ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UDevbladeGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UDevbladeAbilitySystemComponent* UDevbladeGameplayAbility::GetPlayerAbilitySystemComponentFromActorInfo() const
{
	return Cast<UDevbladeAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UDevbladeGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* Target,
	const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	check(TargetASC && InSpecHandle.IsValid()); 
	return GetPlayerAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
		);
}

FActiveGameplayEffectHandle UDevbladeGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* Target,
	const FGameplayEffectSpecHandle& InSpecHandle, EDevbladeSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(Target, InSpecHandle);
	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied()? EDevbladeSuccessType::Successfull : EDevbladeSuccessType::Failed;
	return ActiveGameplayEffectHandle;
}

void UDevbladeGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,
	const TArray<FHitResult>& InHitResults)
{if (InHitResults.IsEmpty())
{
	return;
}
 
	APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());
 
	for (const FHitResult& Hit : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
		{
			if (UDevbladeFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(HitPawn,InSpecHandle);
 
				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{	
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target = HitPawn;
 					    
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						DevbladeGameplayTags::Shared_Event_HitReact,
						Data
					);
				}
			}
		}
	}
}
