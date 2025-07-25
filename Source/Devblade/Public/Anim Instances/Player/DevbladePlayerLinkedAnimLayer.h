// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Anim Instances/DevbladeBaseAnimInstance.h"
#include "DevbladePlayerLinkedAnimLayer.generated.h"

class UDevbladePlayerAnimInstance;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladePlayerLinkedAnimLayer : public UDevbladeBaseAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UDevbladePlayerAnimInstance* GetPlayerAnimInstance() const;
	
};
