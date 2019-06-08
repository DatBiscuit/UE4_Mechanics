// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class DEV_TEMPLATE_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Where to spawn the object
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	//Find a random point within the boxcomponent
	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetRandomPointInVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**The object to spawn
	* Can also change it to a subclass they created using
	* TSubclassOf<class <Name of their class>> WhatToSpawn;
	*/
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APickUpBase> WhatToSpawn;

	FTimerHandle SpawnTimer;

	//Minimum Spawn Delay Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnDelayRangeLow;

	//Maximum Spawn Delay Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnDelayRangeHigh;
	

private:

	/**Box component to specify where pickups should be spawned*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WhereToSpawn;

	/**Handle spawning a object*/
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnObject();

	/**Current spawn delay*/
	float SpawnDelay;
	
};
