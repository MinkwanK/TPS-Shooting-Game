// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Grenade.generated.h"

UCLASS()
class PROJECTA_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//폭팔 함수
	void GrenadeExplosion();
	void FireRay();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite);
	USphereComponent* _CollisionComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite);
	USphereComponent* _GrenadeExplosionCollisionComp;
	
	//폭팔 파티클
	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* _particleComp;

	//폭팔 시간 타이머 핸들
	FTimerHandle _explosionTimerHandle;


	UPROPERTY(EditDefaultsOnly)
	USoundBase* _explosionSound;
	
	int _explosinTime = 4.0f;

};
