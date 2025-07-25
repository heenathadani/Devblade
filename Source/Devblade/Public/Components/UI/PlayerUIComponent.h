// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponentBase.h"
#include "GameplayTagContainer.h"
#include "PlayerUIComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate,TSoftObjectPtr<UTexture2D>,SoftWeaponIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate,FGameplayTag,AbilityInputTag,TSoftObjectPtr<UMaterialInterface>,SoftAbilityIconMaterial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate,FGameplayTag,AbilityInputTag,float,TotalCooldownTime,float,RemainingCooldownTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoneInteractedDelegate,bool,bShouldDisplayInputKey);

UCLASS()
class DEVBLADE_API UPlayerUIComponent : public UPawnUIComponentBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;
	
	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;

	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdated;

	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin;

	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnStoneInteractedDelegate OnStoneInteracted;
};
