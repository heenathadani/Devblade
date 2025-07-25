// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "PlayerTypes/DevbladeStructTypes.h"
#include "DataAsset_PlayerStartUpData.generated.h"

class UDevbladeGameplayAbility;


/**
 * 
 */
UCLASS()
class DEVBLADE_API UDataAsset_PlayerStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UDevbladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FDevbladePlayerAbilitySet> PlayerStartupAbilitySets;
	
	
};
