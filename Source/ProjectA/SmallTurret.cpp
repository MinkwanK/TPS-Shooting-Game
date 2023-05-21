// Fill out your copyright notice in the Description page of Project Settings.

//플레이어가 설치할 수 있는 소구경 터렛
//범위 내의 적들에게 탄약을 발사한다. 탄약은 거리별 일정 오차범위내로 사격 실시한다.
//탄약이 다 떨어졌을 경우 Player가 접근하여 탄약을 보충해줘야한다.


#include "SmallTurret.h"


#include "DrawDebugHelpers.h"
#include "MyCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


/*
 onTargetPerception으로 필요한 Target 정보를 받는다. 이를 target에 등록
 포탑을 target 방향으로 회전. 회전 각도가 일치하면 사격 실시.
 사격은 0.15초 간격으로 번갈아 실시
 */
ASmallTurret::ASmallTurret()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	_hp = 100;
	_ammo = _maxAmmo;
	
	_aiPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	_aiPerceptionStimulSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPercetionStimulSorceComp"));

	_sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	_sight->SightRadius = 5000.0f;
	//인지한 액터를 바로 잊어버리기 때문에 빠르게 적 탐지 가능
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

	_aiPerceptionComp->ConfigureSense(*_sight);
	_aiPerceptionComp->SetDominantSense(*_sight->GetSenseImplementation());

	_aiPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this,&ASmallTurret::OnTargetPerception);
	//_aiPerceptionComp->OnPerceptionUpdated.AddDynamic(this,&ASmallTurret::OnPerceptionUpdated);
	

	_Owner = nullptr;
	_targetEnemy = nullptr;
	_bCanFire = false;

	

	
}

// Called when the game starts or when spawned
void ASmallTurret::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp,Display,TEXT("Begin"));

	_gameStageTimer = Cast<AGameStageTimer>(UGameplayStatics::GetActorOfClass(GetWorld(),AGameStageTimer::StaticClass()));
	_Owner = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	
	_turretRotation = GetActorRotation();
	_gunL = GetMesh()->GetSocketLocation("gunL");
	_gunR = GetMesh()->GetSocketLocation("gunR");

	
}

// 사격 가능 상태인지를 매번 확인하고 사격 실시.
void ASmallTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp,Log,TEXT("turret hp %d"),_hp)
	if(_targetEnemy != nullptr)
	{
		//현재 게임 시작 상태가 아니라면?
	
			Aim();
		
	}

	if(_gameStageTimer != nullptr)
	{
		if(_gameStageTimer->InGameEnum ==  EInGameState::GameRestState)
		{
			_targetEnemy = nullptr;
		}
	}



}

// Called to bind functionality to input
void ASmallTurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ASmallTurret::OnTargetPerception(AActor* Actor, FAIStimulus Stimulus)
{
	
	//인지한 Actor가 Enemy라면 타겟 설정
	//게임 시작 상태에서만 타겟 인지
	if(_targetEnemy == nullptr)
	{
		if(Cast<AEnemy>(Actor) != nullptr)
		{
		
			if(Cast<AEnemy>(Actor)->Controller != nullptr)
			{
				//인지한 Actor를 타겟 배열에 Push
				_targetEnemy = Cast<AEnemy>(Actor);
		
			}
		
	
		}
	}
	
}

void ASmallTurret::Aim()
{

	//UE_LOG(LogTemp,Display,TEXT("Aim"));
	//Target Vec - myVec은 방향 위치를 나타낸다.
	//방향벡터를 Rotation으로 전환
	_targetDirection = _targetEnemy->GetActorLocation() - GetActorLocation();
	_targetRotation = _targetDirection.Rotation();
	
	//터렛을 사실적으로 천천히 회전하고싶다.
	//터렛이 타겟 액터를 완벽히 조준 했을 때 사격 실시.
	
	Turn_Turret();
	
	

}

//터렛을 천천히 타겟을 향해 회전시킨다. (수정 필요)
void ASmallTurret::Turn_Turret()
{

	
	if(_targetRotation.Pitch > _turretRotation.Pitch)
	{
		_turretRotation.Pitch += 0.1f;
	}
	else
	{
		_turretRotation.Pitch -= 0.1f;
	}
	
	if(_targetRotation.Yaw < 0 )
		_targetRotation.Yaw += 360;

	
	if(_targetRotation.Yaw > _turretRotation.Yaw)
	{
		_turretRotation.Yaw += 1.0f;
	}
	else
	{
		_turretRotation.Yaw -= 1.0f;
	}

	const int resultYaw = _turretRotation.Yaw - _targetRotation.Yaw;
	const int resultPitch = _turretRotation.Pitch - _targetRotation.Pitch;
	 UE_LOG(LogTemp,Display,TEXT("Target Rotation Yaw:: %f, Turret Rotation Yaw:: %f"),_targetRotation.Yaw,_turretRotation.Yaw);
	 UE_LOG(LogTemp,Display,TEXT("resultYaw :: %d"),resultYaw);
	// UE_LOG(LogTemp,Display,TEXT("Target Rotation Pitch:: %f, Turret Rotation Pitch:: %f"),_targetRotation.Pitch,_turretRotation.Pitch);
	//UE_LOG(LogTemp,Display,TEXT("resultPitch :: %d"),resultPitch);

	if(resultYaw == 0 && resultPitch == 0)
	{
		_bCanFire = true;
	}
	else
	{
		_bCanFire = false;
	}

	if(_bCanFire)
	{
		//타이머가 작동중이라면?
		if ( !GetWorldTimerManager().IsTimerActive(_fireTimerHandle) ) 
		{
			GetWorldTimerManager().SetTimer(_fireTimerHandle,this,&ASmallTurret::Fire,0.15f,false,0.15f);
		}
	}

	
}


void ASmallTurret::Fire()
{
	for(int i=0;i<2;i++)
	{
		if(_targetEnemy!=nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this->GetWorld(),_fireSound,GetActorLocation());
			_gunL = GetMesh()->GetSocketLocation("gunL");
			_gunR = GetMesh()->GetSocketLocation("gunR");


			//UE_LOG(LogTemp,Display,TEXT("Fire"));
			FHitResult Hit;

			FVector Start = _gunL;
			if(i==1)
				Start = _gunR;
			
			FVector End =  _targetEnemy->GetActorLocation();

			ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
			FCollisionQueryParams QueryParams;

			QueryParams.AddIgnoredActor(this);
	
			GetWorld()->LineTraceSingleByChannel(Hit,Start,End,Channel,QueryParams);

			DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,1.0f);

			if(Hit.GetActor() != nullptr)
			{
				if(Hit.GetActor()->ActorHasTag("Enemy"))
				{
					AEnemy* hitEnemy = Cast<AEnemy>(Hit.GetActor());
					hitEnemy->DecreaseHP(_damage);
		
					if(hitEnemy->_hp <=0)
					{
						_targetEnemy = nullptr;
						_bCanFire = false;
						_Owner->_money += 10;

					}
		
				}
			}
		}
	}
}


void ASmallTurret::DecreaseHP(int value)
{
	_hp -= value;

	if(_hp <= 0)
	{
		_hp = 0;
		_bDead = true;
		
		this->Controller->Destroy();
		this->_aiPerceptionComp->DestroyComponent();
	}
}