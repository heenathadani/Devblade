// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Items/Weapons/BaseWeapon.h"
#include "PlayerTypes/DevbladeStructTypes.h"
#include "PlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DEVBLADE_API APlayerWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "WeaponData")
	FDevbladePlayerWeaponData PlayerWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	const TArray<FGameplayAbilitySpecHandle>& GetGrantedAbilitySpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
	
};
