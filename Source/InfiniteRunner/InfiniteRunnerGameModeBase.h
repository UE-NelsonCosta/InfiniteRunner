// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteRunnerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class INFINITERUNNER_API AInfiniteRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AInfiniteRunnerGameModeBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	bool IsThereACharacterAlive();

	// Due To The Logic Killing The Actor Outright, We Can Just Check For This Variable
	UPROPERTY()
	TArray<AActor*> Characters;
};
