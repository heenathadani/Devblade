// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PlayerTypes/DevbladeEnumTypes.h"
#include "DevbladeGameplayAbility.generated.h"

class UPawnCombatComponent;
class UDevbladeAbilitySystemComponent;

UENUM(BlueprintType)
enum class EDevbladeAbilityActivationPolicy: uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;	
	//~ End UGameplayAbility Interface.

	UPROPERTY(EditDefaultsOnly, Category="Devblade Ability")
	EDevbladeAbilityActivationPolicy AbilityActivationPolicy = EDevbladeAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category="Devblade Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category="Devblade Ability")
	UDevbladeAbilitySystemComponent* GetPlayerAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* Target, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category="Devblade Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* Target, const FGameplayEffectSpecHandle& InSpecHandle, EDevbladeSuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable, Category = "Devblade|Ability")
	void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,const TArray<FHitResult>& InHitResults);

};
