// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSpawnerManager.generated.h"

class APlayerStart;

UCLASS(HideCategories=(Replication, Rendering, Collision, Input, Actor, LOD, Cooking))
class INFINITERUNNER_API ACharacterSpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSpawnerManager();

	UFUNCTION(BlueprintCallable)
	ACharacter* GetCharacterToTheLeft();
	UFUNCTION(BlueprintCallable)
	ACharacter* GetCurrentCharacter();
	UFUNCTION(BlueprintCallable)
	ACharacter* GetCharacterToTheRight();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	 *#if UE_EDITOR

	// Simple Objects 
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		int i = 0; i ++;
	}

	// Arrays
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& PropertyChangedEvent )
	{
		int i = 0; i ++;
		
	}
	#endif
	*/

	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<APlayerStart*> PlayerStarts;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<TSubclassOf<ACharacter>> CharacterClassesToSpawn;
	UPROPERTY(BlueprintReadOnly)
	TArray<ACharacter*> SpawnedCharacters;

	int CurrentCharacter = 0;
};
