// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerTypes/DevbladeEnumTypes.h"
#include "DevbladeSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladeSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	EDevbladeGameDifficulty SavedCurrentGameDifficulty;
};
