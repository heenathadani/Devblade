// All rights reserved by Heena Thadani.


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/WidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}
 
	for (UWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}
	EnemyDrawnWidgets.Empty();
}
