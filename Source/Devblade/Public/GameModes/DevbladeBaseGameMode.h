// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerTypes/DevbladeEnumTypes.h"
#include "DevbladeBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEVBLADE_API ADevbladeBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ADevbladeBaseGameMode();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EDevbladeGameDifficulty CurrentGameDifficulty;
 
public:
	FORCEINLINE EDevbladeGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty;}
};
