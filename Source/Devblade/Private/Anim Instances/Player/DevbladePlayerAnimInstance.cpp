// All rights reserved by Heena Thadani.


#include "Anim Instances/Player/DevbladePlayerAnimInstance.h"

#include "Characters/DevbladePlayer.h"

void UDevbladePlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if(OwningCharacter)
	{
		OwningPlayer = Cast<ADevbladePlayer>(OwningCharacter);
	}
}

void UDevbladePlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if(bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = IdleElapsedTime >= EnterRelaxStateThreshold;
	}
}
