// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretSpawnPoint.h"

// Sets default values
ATurretSpawnPoint::ATurretSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bSpawn  = false;
}

// Called when the game starts or when spawned
void ATurretSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ATurretSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurretSpawnPoint::SpawnTurret()
{
	
	GetWorld()->SpawnActorAbsolute(spawnClass,GetActorTransform());
	this->Destroy();
}


