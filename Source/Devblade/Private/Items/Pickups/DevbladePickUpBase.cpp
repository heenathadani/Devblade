// All rights reserved by Heena Thadani.


#include "Items/Pickups/DevbladePickUpBase.h"
#include "Components/SphereComponent.h"

ADevbladePickUpBase::ADevbladePickUpBase()
{
	PrimaryActorTick.bCanEverTick = false;
 
	PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollisionSphere"));
	SetRootComponent(PickUpCollisionSphere);
	PickUpCollisionSphere->InitSphereRadius(50.f);
	PickUpCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnPickUpCollisionSphereBeginOverlap);

}

void ADevbladePickUpBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
}

