// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Anim Instances/DevbladeCharacterAnimInstance.h"
#include "DevbladePlayerAnimInstance.generated.h"

class ADevbladePlayer;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladePlayerAnimInstance : public UDevbladeCharacterAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	ADevbladePlayer* OwningPlayer;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;
	float IdleElapsedTime;
};
