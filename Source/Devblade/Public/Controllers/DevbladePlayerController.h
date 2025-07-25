// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "DevbladePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEVBLADE_API ADevbladePlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADevbladePlayerController();

	// ~Begin IGenericTeamAgentInterface
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~End IGenericTeamAgentInterface

private:
	FGenericTeamId PlayerTeamId;
	
};
