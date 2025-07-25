// All rights reserved by Heena Thadani.


#include "Anim Instances/DevbladeBaseAnimInstance.h"
#include "DevbladeFunctionLibrary.h"
 
 bool UDevbladeBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
 {
 	if (APawn* OwningPawn = TryGetPawnOwner())
 	{
 		return UDevbladeFunctionLibrary::NativeDoesActorHaveTag(OwningPawn,TagToCheck);
 	}
 
 	return false;
 }
