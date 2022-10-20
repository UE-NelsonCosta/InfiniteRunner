#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelChunkSpawner.generated.h"

UCLASS(HideCategories=("Replication", "Rendering", "LOD", "Collision", "Input", "Actor", "Cooking"))
class INFINITERUNNER_API ALevelChunkSpawner : public AActor
{
	GENERATED_BODY()
	
public: // Constructor
	ALevelChunkSpawner();

protected: // UE Virtual Functions
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private: // Private Functions
	void CacheBlockSize(UWorld& World);
	void GenerateLineOfBlocks(UWorld& World, float& SpawnLocation, float BlockSize);
	void GenerateBlocker(UWorld& World, const FVector& MiddleBlock, const FVector& Bounds) const;
	
	bool CanGenerateABlockerObject(int BlocksSpawned) const;
	bool IsInRangeToSpawnABlock(const APlayerController& PlayerController, float SpawnedBlockXPosition, float MaxSpawnRange) const;
	TSubclassOf<AActor> GetWrappedBlock();

private: // Editor Variables
	UPROPERTY(EditInstanceOnly, Category="LevelChunkSpawner")
	TArray<TSubclassOf<AActor>> ActorClassesToSpawn;

	UPROPERTY(EditInstanceOnly, Category="LevelChunkSpawner")
	float SpawnRange;

	UPROPERTY(EditInstanceOnly, Category="LevelChunkSpawner")
	bool bCreateAllBlocksAtTheStart;

	UPROPERTY(EditInstanceOnly, Category="LevelChunkSpawner|BlockerSpawningParams")
	TSubclassOf<AActor> BlockerBlueprint;

	UPROPERTY(EditInstanceOnly, Category="LevelChunkSpawner|BlockerSpawningParams")
	int MinimumFloorTilesBeforeSpawning;

	UPROPERTY(EditInstanceOnly, Category="LevelChunkSpawner|BlockerSpawningParams")
	int SpaceBetweenBlocks;

private: // Cached Pointers 
	UPROPERTY()
	UWorld* CachedWorld;
	UPROPERTY()
	APlayerController* PlayerControllerToFollow;

private: // Internal Variables
	int SpawnCounter;
	float LastPositionOfSpawnedBlock;
	int CurrentSpawnableBlockIndex;
	FVector BlockBounds;
};
