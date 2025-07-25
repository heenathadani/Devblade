// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponentBase.h"
#include "EnemyUIComponent.generated.h"

class UWidgetBase;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UEnemyUIComponent : public UPawnUIComponentBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UWidgetBase* InWidgetToRegister);
 
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetsIfAny();
 
private:
	TArray<UWidgetBase*> EnemyDrawnWidgets;
};
