// All rights reserved by Heena Thadani.


#include "Characters/DevbladeBaseCharacter.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Ability System/DevbladeAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
ADevbladeBaseCharacter::ADevbladeBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	DevbladeAbilitySystemComponent = CreateDefaultSubobject<UDevbladeAbilitySystemComponent>(TEXT("DevbladeAbilitySystemComponent"));
	DevbladeAttributeSet = CreateDefaultSubobject<UDevbladeAttributeSet>(TEXT("DevbladeAttributeSet"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* ADevbladeBaseCharacter::GetAbilitySystemComponent() const
{
	return GetDevbladeAbilitySystemComponent();
}

UPawnCombatComponent* ADevbladeBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponentBase* ADevbladeBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void ADevbladeBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(DevbladeAbilitySystemComponent)
	{
		DevbladeAbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign startup data to %s"), *GetName());
	}
}

