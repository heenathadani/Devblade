// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DevbladePickUpBase.generated.h"

class USphereComponent;
UCLASS()
class DEVBLADE_API ADevbladePickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADevbladePickUpBase();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick UP Interaction")
	USphereComponent* PickUpCollisionSphere;
 
	UFUNCTION()
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
