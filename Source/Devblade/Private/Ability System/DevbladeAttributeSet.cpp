// All rights reserved by Heena Thadani.


#include "Ability System/DevbladeAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "DevbladeFunctionLibrary.h"
#include "DevbladeGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponentBase.h"
#include "Components/UI/PlayerUIComponent.h"

UDevbladeAttributeSet::UDevbladeAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UDevbladeAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if(!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	
	checkf(CachedPawnUIInterface.IsValid(),TEXT("%s didnt implement IPawnUIInterface"),*Data.Target.GetAvatarActor()->GetActorNameOrLabel());
	
	UPawnUIComponentBase* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent,TEXT("Couldnt extract pawn ui component from %s "),*Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	
	if(Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.0f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
	}
	if(Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.0f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);

		if (GetCurrentRage() == GetMaxRage())
		{
			UDevbladeFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),DevbladeGameplayTags::Player_Status_Rage_Full);
		}
		else if (GetCurrentRage() == 0.f)
		{
			UDevbladeFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),DevbladeGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UDevbladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),DevbladeGameplayTags::Player_Status_Rage_Full);
			UDevbladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),DevbladeGameplayTags::Player_Status_Rage_None);
		}
		
		if(UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage()/GetMaxRage());
		}
	}
	if(Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.0f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		//ToDo: Notify the UI about the damage taken

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
		
		if(GetCurrentHealth() == 0.f)
		{
			UDevbladeFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),DevbladeGameplayTags::Shared_Status_Dead);
		}
	}
}
