// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Dev_TemplateCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Engine/World.h"
#include "Classes/Components/SphereComponent.h"
#include "PickUpBase.h"
#include "ItemPickup.h"
#include "CableComponent.h"
#include "Public/TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// ADev_TemplateCharacter

ADev_TemplateCharacter::ADev_TemplateCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create the collection sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);

	//Set a base health level for the character
	InitialHealth = 200.0f;
	CharacterHealth = InitialHealth;

	//Set the dependences of the speed on Stamina
	InitialStamina = 100.0f;
	CharacterStamina = InitialStamina;
	SpeedFactor = 2.0f;
	BaseSpeed = GetCharacterMovement()->MaxWalkSpeed;
	IsSprinting = false;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADev_TemplateCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADev_TemplateCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADev_TemplateCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADev_TemplateCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADev_TemplateCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADev_TemplateCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADev_TemplateCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADev_TemplateCharacter::OnResetVR);

	//////////////////////////////////////////////////////////////////////

	//Custom Input created using c++ code
	//PlayerInputComponent->BindAction("Collect", IE_Released, this, &ADev_TemplateCharacter::CollectPickups);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADev_TemplateCharacter::CharacterSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADev_TemplateCharacter::StopCharacterSprint);
}


void ADev_TemplateCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADev_TemplateCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ADev_TemplateCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ADev_TemplateCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADev_TemplateCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADev_TemplateCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADev_TemplateCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/**
void ADev_TemplateCharacter::CollectPickups() {
	
	//Get all overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	float CollectedHealth = 0;

	//For each actor we collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected) {

		//cast the actor to APickUpBase
		APickUpBase* const TestPickup = Cast<APickUpBase>(CollectedActors[iCollected]);
		//if the cast is successful and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive()) {
			//Call the pickup's was collected function
			TestPickup->WasCollected();

			//Check to see if the pickup is also a health pickup
			AItemPickup* const TestHealth = Cast<AItemPickup>(TestPickup);
			if (TestHealth) {
				//increase the collected health
				CollectedHealth += TestHealth->GetPower();
			}

			//Deactivate the pickup
			TestPickup->SetActive(false);
		}
	}

	if (CollectedHealth > 0) {
		UpdateHealth(CollectedHealth);
	}
}
**/


/**
void ADev_TemplateCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogClass, Log, TEXT("Overlap occurred"));
	// Overlapped occurred

	//Cast the actor to APickUpBase
	APickUpBase* const TestPickup = Cast<APickUpBase>(OverlappedComponent);
	//if the cast is successful and the pickup is valid and active
	if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive()) {
		//Check the type of pickup and update stats accordingly
		AItemPickup* const TestType = Cast<AItemPickup>(TestPickup);
		if (TestType) {
			if (TestType->GetType().Equals(TEXT("Stamina"))) {
				UpdateStamina(TestType->GetPower());
			}
		}

		//call the pickup's was collected function
		TestPickup->WasCollected();


	}
}
*/

//Reports starting health
float ADev_TemplateCharacter::GetInitialHealth() {
	return InitialHealth;
}

//Reports current health
float ADev_TemplateCharacter::GetCurrentHealth() {
	return CharacterHealth;
}

//Update the character's health
void ADev_TemplateCharacter::UpdateHealth(float HealthChange) {
	CharacterHealth += HealthChange;
}

//Get Initial stamina of the character
float ADev_TemplateCharacter::GetInitialStamina() {
	return InitialStamina;
}

//Get the current stamina of the character
float ADev_TemplateCharacter::GetCurrentStamina() {
	return CharacterStamina;
}

void ADev_TemplateCharacter::UpdateStamina(float StaminaUpdate) {
	//Change the stamina level of the character
		CharacterStamina = GetCurrentStamina() + StaminaUpdate;
		//Making sure we do not go beyond the maximum stamina capacity
		if (GetCurrentStamina() > 100) {
			CharacterStamina = 100;
		}
		//Making sure we do not go below 0 on stamina
		if (GetCurrentStamina() < 0) {
			CharacterStamina = 0;
		}

}

void ADev_TemplateCharacter::CharacterSprint() {

	//Change the speed so the character sprints
	GetCharacterMovement()->MaxWalkSpeed *= SpeedFactor;
	IsSprinting = true;
	//UpdateStamina();

}

void ADev_TemplateCharacter::StopCharacterSprint() {

	//Change the speed so the character STOPS sprinting
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	IsSprinting = false;

}

// Called every frame
void ADev_TemplateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the character is sprinting
	if (IsSprinting) {

		//Condition if the character is out of stamina
		if (GetCurrentStamina() <= 0) {
			//Force the character to stop sprinting
			StopCharacterSprint();
		}
		else {
			//Then decrease the stamina
			UpdateStamina(-0.5f);
		}
	}
	//If the character is not sprinting
	if (GetCurrentStamina() < 100 && !IsSprinting) {
		//then replenish the staminia over time
		UpdateStamina(0.25f);
	}

}