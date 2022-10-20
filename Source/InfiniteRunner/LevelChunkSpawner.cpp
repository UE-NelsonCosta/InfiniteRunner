#include "LevelChunkSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#pragma optimize("", off)

ALevelChunkSpawner::ALevelChunkSpawner()
	: SpawnRange(4000)
	, bCreateAllBlocksAtTheStart(false)
	, MinimumFloorTilesBeforeSpawning(5)
	, SpaceBetweenBlocks(3)
	, CachedWorld(nullptr)
	, PlayerControllerToFollow(nullptr)
	, SpawnCounter(0)
	, LastPositionOfSpawnedBlock(0)
	, CurrentSpawnableBlockIndex(0)
	, BlockBounds(400)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelChunkSpawner::BeginPlay()
{
	Super::BeginPlay();

	if(ActorClassesToSpawn.Num() <= 0)
	{
		PrimaryActorTick.bCanEverTick = false;
		return;
	}

	if(UWorld* World = GetWorld())
	{
		// Cache whatever information we need
		CachedWorld = World;
		CacheBlockSize(*CachedWorld);

		// Find That PlayerController
		PlayerControllerToFollow = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if(!PlayerControllerToFollow)
			return;
		
		if(bCreateAllBlocksAtTheStart)
		{
			while(IsInRangeToSpawnABlock(*PlayerControllerToFollow, LastPositionOfSpawnedBlock, SpawnRange))
			{
				GenerateLineOfBlocks(*CachedWorld, LastPositionOfSpawnedBlock, BlockBounds.X);

				if(CanGenerateABlockerObject(SpawnCounter))
					GenerateBlocker(*CachedWorld, FVector(LastPositionOfSpawnedBlock, 0, 0), BlockBounds);
			}
		}
	}
}

void ALevelChunkSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!PlayerControllerToFollow || !GetWorld())
		return;

	if(IsInRangeToSpawnABlock(*PlayerControllerToFollow, LastPositionOfSpawnedBlock, SpawnRange))
	{
		GenerateLineOfBlocks(*GetWorld(), LastPositionOfSpawnedBlock, BlockBounds.X);

		if(CanGenerateABlockerObject(SpawnCounter))
			GenerateBlocker(*CachedWorld, FVector(LastPositionOfSpawnedBlock, 0, 0), BlockBounds);
	}
}

void ALevelChunkSpawner::CacheBlockSize(UWorld& World)
{
	const FVector Location = FVector::ZeroVector;
	const FRotator Rotator = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// This is how you get it at runtime, but you can use an OnPropertyChanged function to do it at editor time
	if(AActor* SpawnedActor = World.SpawnActor(GetWrappedBlock(), &Location, &Rotator, SpawnParams))
	{
		// Get The Actor Bounds
		FVector Origin = FVector::ZeroVector;
		SpawnedActor->GetActorBounds(true, Origin, BlockBounds, false);

		// Then Let's Just Destroy It
		SpawnedActor->Destroy();
	}

	// Reset This Variable So It Spawns in the right pattern again
	CurrentSpawnableBlockIndex = 0;
}

void ALevelChunkSpawner::GenerateLineOfBlocks(UWorld& World, float& SpawnLocation, const float BlockSize)
{
	const float BlockDiameter = BlockSize * 2;
	
	// Create The Middle Block Location
	const FVector MiddleBlock = FVector(SpawnLocation, 0, 0);

	// Create The Left And Right Blocks Relative To Middle Block
	const FVector LeftBlock   = FVector(0, -BlockDiameter, 0) + MiddleBlock;
	const FVector RightBlock  = FVector(0,  BlockDiameter, 0) + MiddleBlock;

	// Try Spawn Them
	// TODO: Add In An Example Of FActorSpawnParameters
	World.SpawnActor(GetWrappedBlock(), &LeftBlock);
	World.SpawnActor(GetWrappedBlock(), &MiddleBlock);
	World.SpawnActor(GetWrappedBlock(), &RightBlock);

	// Finally Increment The SpawnLocation Variable
	SpawnLocation += BlockDiameter;

	++SpawnCounter;
}

void ALevelChunkSpawner::GenerateBlocker(UWorld& World, const FVector& MiddleBlock, const FVector& Bounds) const
{
	// TODO: Make This A Variable We Can Tweak So That We Generate Variable Numbers Of Blocks Sideways
	// This 4 for now just means that it is in the range of 3, aka -1 0 1
	const int Range = UKismetMathLibrary::RandomInteger(3);
	// Put this back to centered 0
	const int SpawnOffset = Range - 1;

	const FVector Location = MiddleBlock + FVector(0, SpawnOffset * Bounds.Y * 2, 0);
	const FRotator Rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	World.SpawnActor(BlockerBlueprint, &Location, &Rotation, SpawnParams);
}

bool ALevelChunkSpawner::CanGenerateABlockerObject(const int BlocksSpawned) const
{
	return BlocksSpawned >= MinimumFloorTilesBeforeSpawning && ((BlocksSpawned % SpaceBetweenBlocks) == 0);
}

bool ALevelChunkSpawner::IsInRangeToSpawnABlock(const APlayerController& PlayerController, const float SpawnedBlockXPosition, const float MaxSpawnRange) const
{
	// Calculate Distance From PlayerController To The Last Block
	// If Less Than spawnRange, then lets spawn a line!
	float Distance = FLT_MAX;
	if(const APawn* Pawn = PlayerControllerToFollow->GetPawn())
	{
		Distance = FMath::Abs(Pawn->GetActorLocation().X - SpawnedBlockXPosition);
	}

	return Distance < MaxSpawnRange;
}

// TODO: NLMDC Rename Variables To Be Clearer And Simpler
TSubclassOf<AActor> ALevelChunkSpawner::GetWrappedBlock()
{
	if(!ActorClassesToSpawn.IsValidIndex(CurrentSpawnableBlockIndex))
		CurrentSpawnableBlockIndex = 0;
		
	return ActorClassesToSpawn[CurrentSpawnableBlockIndex++];
}

#pragma optimize("", on)
