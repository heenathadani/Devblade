// All rights reserved by Heena Thadani.


#include "Controllers/DevbladePlayerController.h"

ADevbladePlayerController::ADevbladePlayerController()
{
	PlayerTeamId = FGenericTeamId(0);
}

FGenericTeamId ADevbladePlayerController::GetGenericTeamId() const
{
	return PlayerTeamId;
}
