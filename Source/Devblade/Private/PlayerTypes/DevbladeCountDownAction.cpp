// All rights reserved by Heena Thadani.


#include "PlayerTypes/DevbladeCountDownAction.h"

void FDevbladeCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountDownOutput = EDevbladeCountDownActionOutput::Cancelled;
 
		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);
 
		return;
	}
 
	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EDevbladeCountDownActionOutput::Completed;
 
		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);
 
		return;
	}
 
	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f? UpdateInterval : Response.ElapsedTime();
 
		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;
 
		CountDownOutput = EDevbladeCountDownActionOutput::Updated;
 
		Response.TriggerLink(ExecutionFunction,OutputLink,CallbackTarget);
 		  
		ElapsedInterval = 0.f;
	}
}

void FDevbladeCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
