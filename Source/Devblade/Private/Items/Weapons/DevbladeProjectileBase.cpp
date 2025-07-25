// All rights reserved by Heena Thadani.


#include "Items/Weapons/DevbladeProjectileBase.h"
#include "Components/BoxComponent.h"
 #include "NiagaraComponent.h"
 #include "GameFramework/ProjectileMovementComponent.h"
#include "DevbladeFunctionLibrary.h"
 #include "DevbladeGameplayTags.h"
 #include "AbilitySystemBlueprintLibrary.h"

ADevbladeProjectileBase::ADevbladeProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
 
	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this,&ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnProjectileBeginOverlap);
 
	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());
 
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 700.f;
	ProjectileMovementComp->MaxSpeed = 900.f;
	ProjectileMovementComp->Velocity = FVector(1.f,0.f,0.f);
	ProjectileMovementComp->ProjectileGravityScale = 0.f;
 
	InitialLifeSpan = 4.f;
}
void ADevbladeProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	}
}

void ADevbladeProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);
 
	APawn* HitPawn = Cast<APawn>(OtherActor);
 
	if (!HitPawn || !UDevbladeFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
	{
		Destroy();
		return;
	}
 
	bool bIsValidBlock = false;
 
	const bool bIsPlayerBlocking = UDevbladeFunctionLibrary::NativeDoesActorHaveTag(HitPawn,DevbladeGameplayTags::Player_Status_Blocking);
 
	if (bIsPlayerBlocking)
	{
		bIsValidBlock = UDevbladeFunctionLibrary::IsValidBlock(this,HitPawn);
	}
 
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;
 
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			DevbladeGameplayTags::Player_Event_SuccessfulBlock,
			Data
		);
	}
	else
	{
		//Apply projectile damage
		HandleApplyProjectileDamage(HitPawn,Data);
	}
 
	Destroy();
}

void ADevbladeProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedActors.Contains(OtherActor))
	{
		return;
	}
 
	OverlappedActors.AddUnique(OtherActor);
 
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{	
		FGameplayEventData Data;
		Data.Instigator = GetInstigator();
		Data.Target = HitPawn;
 
		if (UDevbladeFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
		{
			HandleApplyProjectileDamage(HitPawn,Data);
		}
	}
}

void ADevbladeProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(),TEXT("Forgot to assign a valid spec handle to the projectile: %s"),*GetActorNameOrLabel());
 
	const bool bWasApplied = UDevbladeFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(),InHitPawn,ProjectileDamageEffectSpecHandle);
 
	if (bWasApplied)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitPawn,
			DevbladeGameplayTags::Shared_Event_HitReact,
			InPayload
		);
	}
}
