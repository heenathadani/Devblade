// All rights reserved by Heena Thadani.


#include "Anim Instances/Player/DevbladePlayerLinkedAnimLayer.h"
#include "Anim Instances/Player/DevbladePlayerAnimInstance.h"

UDevbladePlayerAnimInstance* UDevbladePlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UDevbladePlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
