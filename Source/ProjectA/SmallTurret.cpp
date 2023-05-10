// Fill out your copyright notice in the Description page of Project Settings.

//플레이어가 설치할 수 있는 소구경 터렛
//범위 내의 적들에게 탄약을 발사한다. 탄약은 거리별 일정 오차범위내로 사격 실시한다.
//탄약이 다 떨어졌을 경우 Player가 접근하여 탄약을 보충해줘야한다.


#include "SmallTurret.h"


#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
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
	_sight->DetectionByAffiliation.bDetectNeutrals = true;


	_aiPerceptionComp->ConfigureSense(*_sight);
	_aiPerceptionComp->SetDominantSense(*_sight->GetSenseImplementation());

	_aiPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this,&ASmallTurret::OnTargetPerception);


	
}

// Called when the game starts or when spawned
void ASmallTurret::BeginPlay()
{
	//UE_LOG(LogTemp,Display,TEXT("Begin"));
	Super::BeginPlay();
	
}

// Called every frame
void ASmallTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASmallTurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ASmallTurret::OnTargetPerception(AActor* Actor, FAIStimulus Stimulus)
{
	//매개변수로 받은 Actor를 Player로 형변환
	_enemy = Cast<AEnemy>(Actor);

	if(_enemy != nullptr)
	{
		UE_LOG(LogTemp,Display,TEXT("Target Percetion Success"));

		//Target Vec - myVec은 방향 위치를 나타낸다.
		_targetDirection = (_enemy->GetActorLocation()-GetActorLocation());
		//방향벡터를 Rotation으로 전환
		SetActorRotation(_targetDirection.Rotation());
		
		
	}
	
}


