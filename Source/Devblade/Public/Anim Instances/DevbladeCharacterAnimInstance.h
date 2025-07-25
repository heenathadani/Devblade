// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Anim Instances/DevbladeBaseAnimInstance.h"
#include "DevbladeCharacterAnimInstance.generated.h"

class ADevbladeBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladeCharacterAnimInstance : public UDevbladeBaseAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY()
	ADevbladeBaseCharacter* OwningCharacter;
	
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};
