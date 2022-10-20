// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteRunnerGameModeBase.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AInfiniteRunnerGameModeBase::AInfiniteRunnerGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInfiniteRunnerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters);
}

void AInfiniteRunnerGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsThereACharacterAlive())
	{
		UGameplayStatics::OpenLevel("Game");
	}
}

bool AInfiniteRunnerGameModeBase::IsThereACharacterAlive()
{
	for(int i = 0; i < Characters.Num(); ++i)
	{
		if(Characters[i])
			return true;
	}
	
	return false;
}
