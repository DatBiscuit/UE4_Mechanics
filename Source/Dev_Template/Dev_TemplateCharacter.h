// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Dev_TemplateCharacter.generated.h"

UCLASS(config=Game)
class ADev_TemplateCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Collection Sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;


public:
	ADev_TemplateCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/**Accessor function for health power*/
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetInitialHealth();

	/**Accessor function for current health*/
	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetCurrentHealth();

	/**Function to update the character's health*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateHealth(float HealthChange);

	/**Accessor function for stamina power*/
	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetInitialStamina();

	/**Accessor function for stamina power*/
	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetCurrentStamina();

	/**Function to update the character's stamina*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateStamina(float StaminaUpdate);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/**
	// Overlap
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);
			*/

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/**Called when we press a key to collect any pickups of the type PickUpBase
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void CollectPickups();
		*/

	//Called when we press a key to have the character sprint
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void CharacterSprint();

	//Called when we press a key to have the character sprint
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void StopCharacterSprint();

	//The starting health level of our character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float InitialHealth;

	//The starting stamina level of our character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float InitialStamina;

	//The increase of speed when sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float SpeedFactor;

	//The original speed variable when not sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float BaseSpeed;

	//Whether or not the character is sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool IsSprinting;

private:
	//Current health level of our character
	UPROPERTY(VisibleAnywhere, Category = "Stats")
		float CharacterHealth;

	//Current health level of our character
	UPROPERTY(VisibleAnywhere, Category = "Stats")
		float CharacterStamina;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns Collection Sphere subobject **/
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }
};

