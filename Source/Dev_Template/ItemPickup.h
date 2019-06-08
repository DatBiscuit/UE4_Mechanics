// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpBase.h"
#include "ItemPickup.generated.h"

/**
 * 
 */
UCLASS()
class DEV_TEMPLATE_API AItemPickup : public APickUpBase
{
	GENERATED_BODY()
	
	

public:
	//Sets default values for this actor's properties
	AItemPickup();

	//Override the WasCollected function - use implementation because its a blueprint native event
	//void WasCollected_Implementation() override;

	/**Public way to access the power amount*/
	UFUNCTION(BlueprintPure, Category = "Pickup")
	float GetPower();

	/**Public way to access the pickup type*/
	UFUNCTION(BlueprintPure, Category = "Pickup")
	FString GetType();


protected:
	/**Set the amount of power of the pickup gives to the character; This can be positive or negative*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (BlueprintProtected = "true"))
		float PickupPower;

	/**Set what kind of pickup this is*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (BlueprintProtected = "true"))
		FString PickupType;


	
};
