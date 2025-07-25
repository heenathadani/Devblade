// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "DevbladeBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladeBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag TagToCheck) const;
};
