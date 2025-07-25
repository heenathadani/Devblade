// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Ability System/Abilities/DevbladePlayerGameplayAbility.h"
#include "PlayerGA_PickUpStones.generated.h"

class ADevbladeStoneBase;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UPlayerGA_PickUpStones : public UDevbladePlayerGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UFUNCTION(BlueprintCallable)
	void CollectStones();

	UFUNCTION(BlueprintCallable)
	void ConsumeStones();

private:
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 50.f;
 
	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.f);
 	
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<	EObjectTypeQuery> > StoneTraceChannel;
 
	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugShape = false;
 
	UPROPERTY()
	TArray<ADevbladeStoneBase*> CollectedStones;
	
};
