// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Base.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/SphereComponent.h"

// Sets default values
AProjectile_Base::AProjectile_Base() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create the collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(200.0f);
	RootComponent = CollisionSphere;

	//Create the static mesh component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;


}

// Called when the game starts or when spawned
void AProjectile_Base::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile_Base::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}

void AProjectile_Base::FireInDirection(const FVector& ShootDirection) {
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

