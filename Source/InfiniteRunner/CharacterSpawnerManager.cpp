// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawnerManager.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

// Sets default values
ACharacterSpawnerManager::ACharacterSpawnerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacterSpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	for(int i = 0; i < PlayerStarts.Num(); ++i)
	{
		FVector SpawnLocation = PlayerStarts[i]->GetActorLocation();	
		FRotator SpawnRotation = PlayerStarts[i]->GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* SpawnedActor = GetWorld()->SpawnActor(CharacterClassesToSpawn[i], &SpawnLocation, &SpawnRotation, SpawnParams);
		if(ACharacter* SpawnedCharacter = Cast<ACharacter>(SpawnedActor))
		{
			SpawnedCharacters.AddUnique(SpawnedCharacter);
		}
	}
	
	CurrentCharacter = SpawnedCharacters.Num() / 2;
}

ACharacter* ACharacterSpawnerManager::GetCharacterToTheLeft()
{
	int targetCharacterIndex = CurrentCharacter - 1;
	if(SpawnedCharacters.IsValidIndex(targetCharacterIndex))
	{
		if(ACharacter* characterAtIndex = SpawnedCharacters[targetCharacterIndex])
		{
			CurrentCharacter = targetCharacterIndex;
			return characterAtIndex;
		}
	}

	return nullptr;
}

ACharacter* ACharacterSpawnerManager::GetCurrentCharacter()
{
	return SpawnedCharacters[CurrentCharacter];
}

ACharacter* ACharacterSpawnerManager::GetCharacterToTheRight()
{
	int targetCharacterIndex = CurrentCharacter + 1;
	if(SpawnedCharacters.IsValidIndex(targetCharacterIndex))
	{
		if(ACharacter* characterAtIndex = SpawnedCharacters[targetCharacterIndex])
		{
			CurrentCharacter = targetCharacterIndex;
			return characterAtIndex;
		}
	}

	return nullptr;
}


