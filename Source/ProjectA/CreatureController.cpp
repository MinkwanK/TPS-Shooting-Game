// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureController.h"

#include "DrawDebugHelpers.h"
#include "Enemy.h"
#include "SmallTurret.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"

/*
	Enemy AI Controller

	Enemy가 스폰되면 Player의 위치를 갱신받아 Player를 추격한다.

*/
ACreatureController::ACreatureController()
{
	_aiPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPercetionStimulSorceComp"));

}

void ACreatureController::BeginPlay()
{
	Super::BeginPlay();

	
	_enemy = Cast<AEnemy>(GetPawn());
	if(GetWorld()->GetFirstPlayerController()!= nullptr)
	{
		if(GetWorld()->GetFirstPlayerController()->GetPawn() !=nullptr)
		_targetPlayer = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}

	_gameStageTimer = Cast<AGameStageTimer>(UGameplayStatics::GetActorOfClass(GetWorld(),AGameStageTimer::StaticClass()));

	
	
}

void ACreatureController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(_gameStageTimer != nullptr)
	{
		if(_gameStageTimer->InGameEnum != EInGameState::GameReadyState)
		{
			if(_targetPlayer != nullptr)
			{
				MoveToTarget();
			}
		}
	}
}



void ACreatureController::MoveToTarget()
{
	if(_targetPlayer!=nullptr)
	{
		MoveToActor(_targetPlayer,-1,false,true,true,nullptr,true);
	}
}








