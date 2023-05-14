// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnEnemyActor.h"

// Sets default values
ASpawnEnemyActor::ASpawnEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(_spawnEnemyTimerHandle,this,&ASpawnEnemyActor::SpawnEnemy,1.5f,true,1.5f);
	
}

// Called every frame
void ASpawnEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASpawnEnemyActor::SpawnEnemy()
{
	AActor* actor = GetWorld()->SpawnActorAbsolute(_spawnClass,GetActorTransform());
	
	
}

