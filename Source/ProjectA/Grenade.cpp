// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	_CollisionComp->InitSphereRadius(120);
	_CollisionComp->SetSimulatePhysics(true);
	RootComponent = _CollisionComp;



	_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	_projectileMovement->UpdatedComponent = _CollisionComp;
	_projectileMovement->ProjectileGravityScale = 1.0f;
	_projectileMovement ->InitialSpeed = 500;
	_projectileMovement->MaxSpeed = 500;
	InitialLifeSpan = 3;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

