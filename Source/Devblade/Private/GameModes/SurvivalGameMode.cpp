// All rights reserved by Heena Thadani.

#include "GameModes/SurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/DevbladeEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include "DevbladeFunctionLibrary.h"

void ASurvivalGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName,Options,ErrorMessage);
 
	EDevbladeGameDifficulty SavedGameDifficulty;
 
	if (UDevbladeFunctionLibrary::TryLoadSavedGameDifficulty(SavedGameDifficulty))
	{
		CurrentGameDifficulty = SavedGameDifficulty;
	}
}

void ASurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
	checkf(EnemyWaveSpawnerDataTable,TEXT("Forgot to assign a valid data table in survival game mode blueprint"));
 
	SetCurrentSurvivalGameModeState(EDevbladeSurvivalGameModeState::WaitSpawnNewWave);
 
	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
	PreLoadNextWaveEnemies();
}

void ASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentSurvivalGameModeState == EDevbladeSurvivalGameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaTime;
 
		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;
 
			SetCurrentSurvivalGameModeState(EDevbladeSurvivalGameModeState::SpawningNewWave);
		}
	}
 
	if (CurrentSurvivalGameModeState == EDevbladeSurvivalGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaTime;
 
		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{

			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
 
			TimePassedSinceStart = 0.f;
 
			SetCurrentSurvivalGameModeState(EDevbladeSurvivalGameModeState::InProgress);
		}
	}
 
	if (CurrentSurvivalGameModeState == EDevbladeSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaTime;
 
		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.f;
 
			CurrentWaveCount++;
 
			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModeState(EDevbladeSurvivalGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModeState(EDevbladeSurvivalGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

bool ASurvivalGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount>TotalWavesToSpawn;
}

void ASurvivalGameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves())
	{
		return;
	}

	PreLoadedEnemyClassMap.Empty();
 
	for (const FDevbladeEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if(SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;
 
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo,this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn,LoadedEnemyClass);
					}
				}
			)
		);
	}
}

FDevbladeEnemyWaveSpawnerTableRow* ASurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
 
	FDevbladeEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FDevbladeEnemyWaveSpawnerTableRow>(RowName,FString());
 	
	checkf(FoundRow,TEXT("Could not find a valid row under the name %s in the data table"),*RowName.ToString());
 
	return FoundRow;
}

int32 ASurvivalGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this,ATargetPoint::StaticClass(),TargetPointsArray);
	}
 	
	checkf(!TargetPointsArray.IsEmpty(),TEXT("No valid target point found in level: %s for spawning enemies"),*GetWorld()->GetName());
 
	uint32 EnemiesSpawnedThisTime = 0;
 
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
 
	for (const FDevbladeEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if(SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;
 
		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount,SpawnerInfo.MaxPerSpawnCount);
 
		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);
 
		for (int32 i = 0; i < NumToSpawn; i++)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0,TargetPointsArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();
 
			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this,SpawnOrigin,RandomLocation,400.f);
 
			RandomLocation += FVector(0.f,0.f,150.f);
 
			ADevbladeEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ADevbladeEnemy>(LoadedEnemyClass,RandomLocation,SpawnRotation,SpawnParam);
 
			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this,&ThisClass::OnEnemyDestroyed);
				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
			}
 			
			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}
 
	return EnemiesSpawnedThisTime;
}

bool ASurvivalGameMode::ShouldKeepSpawnEnemies() const
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void ASurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	CurrentSpawnedEnemiesCounter--;
	
	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	else if (CurrentSpawnedEnemiesCounter == 0)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;
 
		SetCurrentSurvivalGameModeState(EDevbladeSurvivalGameModeState::WaveCompleted);
	}
}

void ASurvivalGameMode::SetCurrentSurvivalGameModeState(EDevbladeSurvivalGameModeState InState)
{
	CurrentSurvivalGameModeState = InState;
	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}

void ASurvivalGameMode::RegisterSpawnedEnemies(const TArray<ADevbladeEnemy*>& InEnemiesToRegister)
{
	for (ADevbladeEnemy* SpawnedEnemy : InEnemiesToRegister)
	{
		if (SpawnedEnemy)
		{
			CurrentSpawnedEnemiesCounter++;
 
			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this,&ThisClass::OnEnemyDestroyed);
		}
	}
}
