// All rights reserved by Heena Thadani.

#pragma once

#include"CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "DevbladeGameInstance.generated.h"

USTRUCT(BlueprintType)
 struct FDevbladeGameLevelSet
{
	GENERATED_BODY()
 
	UPROPERTY(EditDefaultsOnly,meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;
 
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;
 
	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}
};

UCLASS()
class DEVBLADE_API UDevbladeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintPure, meta =(GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag) const;
	
protected:
	virtual void OnPreLoadMap(const FString& MapName);
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FDevbladeGameLevelSet> GameLevelSets;
};
