// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnUIInterface.generated.h"

class UEnemyUIComponent;
class UPawnUIComponentBase;
class UPlayerUIComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEVBLADE_API IPawnUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UPawnUIComponentBase* GetPawnUIComponent() const = 0;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const;
};
