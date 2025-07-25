// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "PlayerTypes/DevbladeEnumTypes.h"

class FDevbladeCountDownAction : public FPendingLatentAction
{
public:
	FDevbladeCountDownAction(float InTotalCountDownTime,float InUpdateInterval,float& InOutRemainingTime,
	EDevbladeCountDownActionOutput& InCountDownOutput,const FLatentActionInfo& LatentInfo):
	bNeedToCancel(false)
	, TotalCountDownTime(InTotalCountDownTime)
	, UpdateInterval(InUpdateInterval)
	, OutRemainingTime(InOutRemainingTime)
	, CountDownOutput(InCountDownOutput)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
	, ElapsedInterval(0.f)
	, ElapsedTimeSinceStart(0.f)
	{
		
	}
	virtual void UpdateOperation(FLatentResponse& Response) override;
	void CancelAction();
private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EDevbladeCountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};
