// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class APlayerWeapon;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UPlayerCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
public:
	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractActor) override;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	APlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	APlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetPlayerCurrentEquippedWeaponDamageAtLevel(float InLevel) const;
};
