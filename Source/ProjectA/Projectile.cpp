// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Enemy.h"

//플레이어의 총에서 발사되는 투사체
//적과 충돌하면 데미지를 준다.
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	_CollisionComp->InitSphereRadius(60);
	_CollisionComp->BodyInstance.SetCollisionProfileName("BlockAll");
	_CollisionComp->SetSimulatePhysics(true);
	RootComponent = _CollisionComp;



	_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	_projectileMovement->UpdatedComponent = _CollisionComp;
	_projectileMovement->ProjectileGravityScale = 0.0f;
	_projectileMovement ->InitialSpeed = 1500;
	_projectileMovement->MaxSpeed = 1500;
	InitialLifeSpan = 3;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//투사체가 적과 충돌하면 체력 차감
void AProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if(Hit.GetActor()!=nullptr)
	{
		//투사체가 적과 충돌한다면?
		if(Hit.GetActor()->ActorHasTag("Enemy"))
		{
			AEnemy* Creature = Cast<AEnemy>(Hit.GetActor());

			//투사체의 주인이 누구인지에 따라서 다른 데미지를 준다.
			if(this->GetOwner() != nullptr)
			{
				if(this->GetOwner()->ActorHasTag("Player"))
				{
					Creature->DecreaseHP(30);
				}
				else if(this->GetOwner()->ActorHasTag("Sniper"))
				{
					Creature->DecreaseHP(150);
				}
			}
		
		}
	}
	
}


