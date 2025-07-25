// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEVBLADE_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
public:
	virtual void OnHitTargetActor(AActor* HitActor) override;

protected:
	virtual void ToggleBodyCollsionBoxCollision(bool bShouldEnable,EToggleDamageType ToggleDamageType) override;
	
};
