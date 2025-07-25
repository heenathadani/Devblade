// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayerTypes/DevbladeEnumTypes.h"
#include "DevbladeFunctionLibrary.generated.h"

class UPawnCombatComponent;
struct FGameplayTag;
class UDevbladeAbilitySystemComponent;
struct FScalableFloat;
class UDevbladeGameInstance;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UDevbladeAbilitySystemComponent* NativeGetDevbladeAscFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Devblade|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Devblade|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Devblade|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck, EDevbladeConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Devblade|FunctionLibrary", meta = (DisplayName = "GetPawnCombatComponentFromActor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor,EDevbladeValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "Devblade|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn,APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Devblade|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat,float InLevel = 1.f);

	UFUNCTION(BlueprintPure, Category = "Devblade|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker,AActor* InVictim,float& OutAngleDifference);

	UFUNCTION(BlueprintPure, Category = "Devblade|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker,AActor* InDefender);
	
	UFUNCTION(BlueprintCallable,Category = "Devblade|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator,AActor* InTargetActor,const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable,Category = "Devblade|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput",TotalTime = "1.0",UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime,
		EDevbladeCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EDevbladeCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintPure, Category = "Devblade|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UDevbladeGameInstance* GetDevbladeGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable,Category = "Devblade|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject,EDevbladeInputMode InInputMode);

	UFUNCTION(BlueprintCallable,Category = "Devblade|FunctionLibrary")
	static void SaveCurrentGameDifficulty(EDevbladeGameDifficulty InDifficultyToSave);
 
	UFUNCTION(BlueprintCallable,Category = "Devblade|FunctionLibrary")
	static bool TryLoadSavedGameDifficulty(EDevbladeGameDifficulty& OutSavedDifficulty);

};
