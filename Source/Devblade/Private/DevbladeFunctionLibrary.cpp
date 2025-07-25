// All rights reserved by Heena Thadani.


#include "DevbladeFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "DevbladeGameplayTags.h"
#include "PlayerTypes/DevbladeCountDownAction.h"
#include "DevbladeGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DevbladeSaveGame.h"

#include "DevbladeDebugHelper.h"

UDevbladeAbilitySystemComponent* UDevbladeFunctionLibrary::NativeGetDevbladeAscFromActor(AActor* InActor)
{
	check(InActor);
	return CastChecked<UDevbladeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UDevbladeFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UDevbladeAbilitySystemComponent* ASC = NativeGetDevbladeAscFromActor(InActor);
	if(!ASC -> HasMatchingGameplayTag(TagToAdd))
	{
		ASC -> AddLooseGameplayTag(TagToAdd);
	};
}

void UDevbladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UDevbladeAbilitySystemComponent* ASC = NativeGetDevbladeAscFromActor(InActor);
	if(ASC -> HasMatchingGameplayTag(TagToRemove))
	{
		ASC -> RemoveLooseGameplayTag(TagToRemove);
	};
}

bool UDevbladeFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UDevbladeAbilitySystemComponent* ASC = NativeGetDevbladeAscFromActor(InActor);
	return ASC -> HasMatchingGameplayTag(TagToCheck);
}

void UDevbladeFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck,
	EDevbladeConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EDevbladeConfirmType :: Yes : EDevbladeConfirmType :: No;
}

UPawnCombatComponent* UDevbladeFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	if(IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface -> GetPawnCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UDevbladeFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EDevbladeValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	OutValidType = CombatComponent ? EDevbladeValidType :: Valid : EDevbladeValidType :: Invalid;
	return CombatComponent;
}

bool UDevbladeFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{ check(QueryPawn && TargetPawn);
 
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
 
	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
 
	return false;
}

float UDevbladeFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UDevbladeFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim,
	float& OutAngleDifference)
{
	check(InAttacker && InVictim);
 
	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();
 
	const float DotResult = FVector::DotProduct(VictimForward,VictimToAttackerNormalized);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);
 
	const FVector CrossResult = FVector::CrossProduct(VictimForward,VictimToAttackerNormalized);
 
	if (CrossResult.Z < 0.f)
	{
		OutAngleDifference *= -1.f;
	}
 
	if (OutAngleDifference>=-45.f && OutAngleDifference <=45.f)
	{
		return DevbladeGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference<-45.f && OutAngleDifference>=-135.f)
	{
		return DevbladeGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDifference<-135.f || OutAngleDifference>135.f)
	{
		return DevbladeGameplayTags::Shared_Status_HitReact_Back;
	}
	else if(OutAngleDifference>45.f && OutAngleDifference<=135.f)
	{
		return DevbladeGameplayTags::Shared_Status_HitReact_Right;
	}
 
	return DevbladeGameplayTags::Shared_Status_HitReact_Front;
}

bool UDevbladeFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);
 
	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(),InDefender->GetActorForwardVector());
 
	/*const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"),DotResult,DotResult<-0.1f? TEXT("Valid Block") : TEXT("InvalidBlock"));
 
	Debug::Print(DebugString,DotResult<-0.1f? FColor::Green : FColor::Red);*/
 
	return DotResult<-0.1f;
}

bool UDevbladeFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{   
	UDevbladeAbilitySystemComponent* SourceAsc = NativeGetDevbladeAscFromActor(InInstigator);
	UDevbladeAbilitySystemComponent* TargetAsc = NativeGetDevbladeAscFromActor(InTargetActor);
 
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceAsc->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data,TargetAsc);
 
	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UDevbladeFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,
	float& OutRemainingTime, EDevbladeCountDownActionInput CountDownInput,
	UPARAM(DisplayName = "Output") EDevbladeCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;
 
	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	}
 
	if (!World)
	{
		return;
	}
 
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
 
	FDevbladeCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FDevbladeCountDownAction>(LatentInfo.CallbackTarget,LatentInfo.UUID);
 
	if (CountDownInput == EDevbladeCountDownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID,
				new FDevbladeCountDownAction(TotalTime,UpdateInterval,OutRemainingTime,CountDownOutput,LatentInfo)
			);
		}
	}
 
	if (CountDownInput == EDevbladeCountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

UDevbladeGameInstance* UDevbladeFunctionLibrary::GetDevbladeGameInstance(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return World->GetGameInstance<UDevbladeGameInstance>();
		}
	}
 
	return nullptr;
}

void UDevbladeFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EDevbladeInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;
 
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}
 
	if (!PlayerController)
	{
		return;
	}
 
	FInputModeGameOnly GameOnlyMode;
	FInputModeUIOnly UIOnlyMode;
 
	switch (InInputMode)
	{
	case EDevbladeInputMode::GameOnly:
         
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;
 
		break;
 
	case EDevbladeInputMode::UIOnly:
 
		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;
 
		break;
 
	default:
		break;
	}
}

void UDevbladeFunctionLibrary::SaveCurrentGameDifficulty(EDevbladeGameDifficulty InDifficultyToSave)
{
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UDevbladeSaveGame::StaticClass());
 
	if (UDevbladeSaveGame* DevbladeSaveGameObject = Cast<UDevbladeSaveGame>(SaveGameObject))
	{
		DevbladeSaveGameObject->SavedCurrentGameDifficulty = InDifficultyToSave;
 
		const bool bWasSaved = UGameplayStatics::SaveGameToSlot(DevbladeSaveGameObject,DevbladeGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(),0);
 
		Debug::Print(bWasSaved? TEXT("Difficulty Saved") : TEXT("Difficulty NOT Saved"));
	}
}

bool UDevbladeFunctionLibrary::TryLoadSavedGameDifficulty(EDevbladeGameDifficulty& OutSavedDifficulty)
{
	if (UGameplayStatics::DoesSaveGameExist(DevbladeGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
	{
		USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(DevbladeGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(),0);
 
		if (UDevbladeSaveGame* DevbladeSaveGameObject = Cast<UDevbladeSaveGame>(SaveGameObject))
		{
			OutSavedDifficulty = DevbladeSaveGameObject->SavedCurrentGameDifficulty;
 
			Debug::Print(TEXT("Loading Successful"),FColor::Green);
 
			return true;
		}
	}
 return false;
}
 