// All rights reserved by Heena Thadani.


#include "DevbladeGameInstance.h"
#include "MoviePlayer.h"

void UDevbladeGameInstance::Init()
{
	Super::Init();
 
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::OnDestinationWorldLoaded);
}

TSoftObjectPtr<UWorld> UDevbladeGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	for (const FDevbladeGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if(!GameLevelSet.IsValid()) continue;
 
		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}
 
	return TSoftObjectPtr<UWorld>();
}

void UDevbladeGameInstance::OnPreLoadMap(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
 
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void UDevbladeGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	GetMoviePlayer()->StopMovie();
}
