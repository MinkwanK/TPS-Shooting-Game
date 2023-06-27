// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

class AEnemy;


//수류탄 스크립트
//스폰될 시, 4초뒤에 폭팔한다.
// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	_CollisionComp->InitSphereRadius(10);
	_CollisionComp->SetSimulatePhysics(true);
	RootComponent = _CollisionComp;

	_GrenadeExplosionCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("GrenadeExplosionCollisionComp"));
	_GrenadeExplosionCollisionComp->InitSphereRadius(150);
	_GrenadeExplosionCollisionComp->SetSimulatePhysics(true);
	_GrenadeExplosionCollisionComp->SetupAttachment(_CollisionComp);
	_GrenadeExplosionCollisionComp->BodyInstance.SetCollisionProfileName("OverlapAll");
	_GrenadeExplosionCollisionComp->SetEnableGravity(false);
	
	_particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	_particleComp->SetupAttachment(_CollisionComp);
	_particleComp->bAutoActivate = false;

	
	
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(_explosionTimerHandle,this,&AGrenade::GrenadeExplosion,_explosinTime);
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void AGrenade::GrenadeExplosion()
{
	UE_LOG(LogTemp,Log,TEXT("Explosion"));
	_particleComp->ToggleActive();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),_explosionSound,GetActorLocation(),GetActorRotation());

	

	
		TArray<UPrimitiveComponent*> overlappedComponents;
	 _GrenadeExplosionCollisionComp->GetOverlappingComponents(overlappedComponents);
	 UE_LOG(LogTemp,Log,TEXT("overlappedComponents : %d"),overlappedComponents.Num());
	
	// //수류탄이 터질때 겹쳐진 컴포넌트들 모으기
	// TArray<UPrimitiveComponent*> overlappedComponents;
	// //_GrenadeExplosionCollisionComp->getover
	// _GrenadeExplosionCollisionComp->GetOverlappingComponents(overlappedComponents);
	// UE_LOG(LogTemp,Log,TEXT("overlappedComponents : %d"),overlappedComponents.Num());
	//
	// //수류탄이 터질때 주변의 적들 및 플레이어 체력 차감
	// for(int i = 0; i < overlappedComponents.Num();i++)
	// {
	// 	if(overlappedComponents[i]->GetOwner()->ActorHasTag("Enemy") || overlappedComponents[i]->GetOwner()->ActorHasTag("Player"))
	// 	{
	// 		if(overlappedComponents[i]->GetOwner()->ActorHasTag("Enemy"))
	// 		{
	// 			AEnemy* enemy = Cast<AEnemy>(overlappedComponents[i]->GetOwner());
	// 			
	// 			if(enemy->_hp>0)
	// 			{
	// 				enemy->DecreaseHP(500);
	// 			}
	// 		}
	// 		else if(overlappedComponents[i]->GetOwner()->ActorHasTag("Player"))
	// 		{
	// 			AMyCharacter* myCharacer = Cast<AMyCharacter>(overlappedComponents[i]->GetOwner());
	// 			
	// 			if(myCharacer->_hp>0)
	// 			{
	// 				myCharacer->DecreaseHP(500);
	// 			}
	// 		}
	// 	}
	// }
	
}





