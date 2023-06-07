// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "DamageActor.h"
#include "Enemy.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

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

	_damage = 0;

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
					//플레이어가 가한 데미지를 텍스트로 표시되게 적의 머리위 로 스폰
					//이를 통해 아군이 얼마나 딜을 줬는지 텍스트 렌더러 액터를 생성할 수 있다.
					SpawnTextRender(Hit);
					
					//몬스터의 체력 차감
					Creature->DecreaseHP(_damage);
				}
				else if(this->GetOwner()->ActorHasTag("Sniper"))
				{

					//아군 스나이퍼가 가한 데미지를 텍스트로 표시되게 적의 머리위 로 스폰
					//이를 통해 아군이 얼마나 딜을 줬는지 텍스트 렌더러 액터를 생성할 수 있다.
					SpawnTextRender(Hit);
					
					Creature->DecreaseHP(_damage);
				}
			}
		
		}
	}
	
}

void AProjectile::SpawnTextRender(FHitResult Hit)
{
	//플레이어가 가한 데미지에 투사체에 적용된 데미지를 적용한다.
	//이를 통해 플레이어는 얼마나 딜을 줬는지 텍스트 렌더러 액터를 생성할 수 있다.
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
					
	//스폰 위치 조정 (몬스터의 머리 위로)
	FVector hitLocation = Hit.Location;
	hitLocation.Z += 20;
	//스폰
	FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(hitLocation,Hit.GetActor()->GetActorRotation(),FVector(1,1,1));
	ADamageActor* damageActor = GetWorld()->SpawnActor<ADamageActor>(_damageActor,SpawnTransform,SpawnParams);
					
					
	if(damageActor!=nullptr)
	{
		//맞은 콜라이더의 태그가 head라면 데미지 두배 주고 빨간색으로 강조
		if(Hit.Component->ComponentHasTag("head"))
		{
			_damage *= 2;
			damageActor->_textRenderComp->SetTextRenderColor(FColor::Red);
		}
		else
		{
			damageActor->_textRenderComp->SetTextRenderColor(FColor::White);
		}
		//데미지 텍스트 적용 및 수명 주기 확정
		damageActor->_textRenderComp->SetText(FString::FromInt(_damage));
		damageActor->SetLifeSpan(0.2f);
	}
}


