// Fill out your copyright notice in the Description page of Project Settings.


#include "Sniper.h"

#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISenseConfig_Sight.h"

//플레이어를 도와 적과 교전하는 스나이퍼 NPC

//생성자 함수, 인지능력 부여
ASniper::ASniper()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	_sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	_sight->SightRadius = 5000.0f;
	_sight->LoseSightRadius = 5000;
	_sight->SetMaxAge(5.0);
	_sight->PeripheralVisionAngleDegrees = 180;
	//범위 내에 이미 인지된 액터를  영원히 기억할지 말지?
	_sight->AutoSuccessRangeFromLastSeenLocation = -1;
	//근거리시야반경
	_sight->NearClippingRadius = 0;
	_sight->DetectionByAffiliation.bDetectNeutrals = true;
	_sight->DetectionByAffiliation.bDetectEnemies = true;
	_sight->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*_sight);
	AIPerceptionComponent->SetDominantSense(*_sight->Implementation);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ASniper::OnTargetPerception);
	_bCanFire = true;

}

// Called when the game starts or when spawned
void ASniper::BeginPlay()
{
	Super::BeginPlay();
	_gameStageTimer = Cast<AGameStageTimer>(UGameplayStatics::GetActorOfClass(GetWorld(),AGameStageTimer::StaticClass()));
	
}

//목표 타겟이 있고 사격 가능하다면 사격
void ASniper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(_targetCreature!=nullptr && _bCanFire)
	{
		Aim();
	}
	
	if(_gameStageTimer->InGameEnum == EInGameState::GameRestState)
	{
		_targetCreature = nullptr;
	}
	
}

void ASniper::OnTargetPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if(Cast<AEnemy>(Actor) != nullptr)
	{
		_targetCreature = Cast<AEnemy>(Actor);
	}
}

void ASniper::Aim()
{
	if(_targetCreature != nullptr && _targetCreature->_hp >0)
	{
		FRotator TargetRoc = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),_targetCreature->GetActorLocation());
		TargetRoc.Pitch = 0;
		TargetRoc.Roll = 0;
		//SmoothTurn(TargetRoc,15);
		SetActorRotation(TargetRoc);
	
		Fire();
	}
}

void ASniper::SmoothTurn(FRotator targetRotation,int InterpSeed)
{

	const FRotator resultRotation = FMath::RInterpTo(GetActorRotation(),targetRotation,GetWorld()->GetDeltaSeconds(),InterpSeed);
		
	SetActorRotation(resultRotation);
}


void ASniper::Fire()
{
	FVector ProjectileSpawnLocation = GetMesh()->GetChildComponent(0)->GetSocketLocation("b_gun_muzzleflash");
	//Start Point에서 Target Point를 바라보는 회전 각도 찾기

	FRotator ProjectileSpawnRotation;

	//Ray가 Hit한 Actor가 있을 때는 그 Actor를 바라보는 방향 각도를 이용하여 스폰.
	if(_targetCreature!=nullptr)
	{
		ProjectileSpawnRotation = UKismetMathLibrary::FindLookAtRotation(ProjectileSpawnLocation,_targetCreature->GetActorLocation());

		FTransform ProjectileSpawnTransform = UKismetMathLibrary::MakeTransform(ProjectileSpawnLocation,ProjectileSpawnRotation,FVector(1,1,1));
	
	
		if(_projectile)
		{
		
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
		
			AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(_projectile,ProjectileSpawnTransform,SpawnParams);
			projectile->SetOwner(this);
			projectile->_damage = 150;
			PlayAnimMontage(_fireMontage);
			if(projectile)
			{
				//투사체가 자기 자신은 무시하도록 하기.
				projectile->_CollisionComp->MoveIgnoreActors.Add(SpawnParams.Owner);
			}

			PlayAnimMontage(_fireMontage);
			_bCanFire = false;
			GetWorldTimerManager().SetTimer(_fireTimerHandle,this,&ASniper::SetFire,1.5f,false);
		}
		
	}
}

