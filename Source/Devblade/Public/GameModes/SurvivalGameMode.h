// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/DevbladeBaseGameMode.h"
#include "SurvivalGameMode.generated.h"

class ADevbladeEnemy;

UENUM(BlueprintType)
enum class EDevbladeSurvivalGameModeState : uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};
USTRUCT(BlueprintType)
struct FDevbladeEnemyWaveSpawnerInfo
{
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ADevbladeEnemy> SoftEnemyClassToSpawn;
 
	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;
 
	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
};
 
USTRUCT(BlueprintType)
struct FDevbladeEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere)
	TArray<FDevbladeEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;
 
	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvialGameModeStateChangedDelegate,EDevbladeSurvivalGameModeState,CurrentState);
/**
 * 
 */
UCLASS()
class DEVBLADE_API ASurvivalGameMode : public ADevbladeBaseGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	bool HasFinishedAllWaves() const;
	void PreLoadNextWaveEnemies();
	FDevbladeEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawnEnemies() const;

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
 
private:
	void SetCurrentSurvivalGameModeState(EDevbladeSurvivalGameModeState InState);
 	
	UPROPERTY()
	EDevbladeSurvivalGameModeState CurrentSurvivalGameModeState;
 
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnSurvialGameModeStateChangedDelegate OnSurvivalGameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;
 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;

	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;
 
	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;
 
	UPROPERTY()
	TArray<AActor*> TargetPointsArray;
 
	UPROPERTY()
	float TimePassedSinceStart = 0.f;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.f;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 2.f;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.f;

	UPROPERTY()
	TMap< TSoftClassPtr < ADevbladeEnemy >,UClass* > PreLoadedEnemyClassMap;
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnedEnemies(const TArray<ADevbladeEnemy*>& InEnemiesToRegister);
};
