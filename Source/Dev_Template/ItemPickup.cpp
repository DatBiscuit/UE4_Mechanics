// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemPickup.h"
#include "Classes/Components/StaticMeshComponent.h"

//Set default values
AItemPickup::AItemPickup() {
	
	/**This will tell the mesh to simulate physics once the game is played
	* If they do not want physics to be simulated they can uncheck it in the editor
	*/
	GetMesh()->SetSimulatePhysics(true);

	//The base power of the pickup
	PickupPower = 150.f;

	//Initialize the pickup type name
	PickupType = TEXT("");

}

/**
void AItemPickup::WasCollected_Implementation() {

	//use the base pickup behavior
	Super::WasCollected_Implementation();

	//Destroy the batter; only want to put this on the battery
	Destroy();
}
*/


//Report the power amount
float AItemPickup::GetPower() {
	return PickupPower;
}

FString AItemPickup::GetType() {
	return PickupType;
}





