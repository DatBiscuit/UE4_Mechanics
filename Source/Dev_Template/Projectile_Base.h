// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Projectile_Base.generated.h"

UCLASS()
class DEV_TEMPLATE_API AProjectile_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* ProjectileMesh;


	/** Collision Sphere */
	UPROPERTY(EditAnywhere)
		class USphereComponent* CollisionSphere;


	/** Projectile Movement */
	UPROPERTY(EditAnywhere, Category = Movement)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	/** Function to handle projectile's velocity in the shoot direction */
	void FireInDirection(const FVector& ShootDirection);

};
