// All rights reserved by Heena Thadani.


#include "Characters/DevbladeEnemy.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Widgets/WidgetBase.h"
#include "DevbladeFunctionLibrary.h"
#include "GameModes/DevbladeBaseGameMode.h"

ADevbladeEnemy::ADevbladeEnemy()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
	EnemyHealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthBarWidgetComponent");
	EnemyHealthBarWidgetComponent->SetupAttachment(GetMesh());

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);
 
	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);
}

UPawnCombatComponent* ADevbladeEnemy::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponentBase* ADevbladeEnemy::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* ADevbladeEnemy::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void ADevbladeEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (UWidgetBase* HealthWidget = Cast<UWidgetBase>(EnemyHealthBarWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreatedWidget(this);
	}
}

void ADevbladeEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitEnemyStartupData();
}

#if WITH_EDITOR
void ADevbladeEnemy::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
 
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,LeftHandCollisionBoxAttachBoneName);
	}
 
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,RightHandCollisionBoxAttachBoneName);
	}
}
#endif


void ADevbladeEnemy::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UDevbladeFunctionLibrary::IsTargetPawnHostile(this,HitPawn))
		{
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
		}
	}
}

void ADevbladeEnemy::InitEnemyStartupData()
{
	if(CharacterStartUpData.IsNull())
	{
		return;
	}
	
	int32 AbilityApplyLevel = 1;
 
	if (ADevbladeBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<ADevbladeBaseGameMode>())
	{
		switch (BaseGameMode->GetCurrentGameDifficulty())
		{
		case EDevbladeGameDifficulty::Easy:
			AbilityApplyLevel = 1;
			break;
 
		case EDevbladeGameDifficulty::Normal:
			AbilityApplyLevel = 2;
			break;
 
		case EDevbladeGameDifficulty::Hard:
			AbilityApplyLevel = 3;
			break;
 
		case EDevbladeGameDifficulty::VeryHard:
			AbilityApplyLevel = 4;
			break;
 
		default:
			break;
		}
	}
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, AbilityApplyLevel]()
			{
				if(UDataAsset_StartUpDataBase* LoadedData= CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(DevbladeAbilitySystemComponent,AbilityApplyLevel);
				}
			}
			)
		);
	
}
