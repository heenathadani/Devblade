// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Items/Pickups/DevbladePickUpBase.h"
#include "DevbladeStoneBase.generated.h"

class UWarriorAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class DEVBLADE_API ADevbladeStoneBase : public ADevbladePickUpBase
{
	GENERATED_BODY()
	
public:
	void Consume(UDevbladeAbilitySystemComponent* AbilitySystemComponent,int32 ApplyLevel);
	
protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();
 
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
