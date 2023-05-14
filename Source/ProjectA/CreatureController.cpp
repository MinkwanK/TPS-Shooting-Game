// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureController.h"

#include "DrawDebugHelpers.h"
#include "Enemy.h"
#include "SmallTurret.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"

/*
	Enemy AI Controller

	Enemy가 스폰되면 Player의 위치를 갱신받아 Player를 추격한다.

*/
ACreatureController::ACreatureController()
{

	// _aIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	_aiPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPercetionStimulSorceComp"));
	
	// _sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	// _sight->SightRadius = 8000.0f;
	// _sight->DetectionByAffiliation.bDetectNeutrals = true;
	// _sight->DetectionByAffiliation.bDetectEnemies = true;
	// _sight->DetectionByAffiliation.bDetectFriendlies = true;
	//
	//
	//
	// _aIPerceptionComponent->ConfigureSense(*_sight);
	// _aIPerceptionComponent->SetDominantSense(*_sight->GetSenseImplementation());
	// _aIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ACreatureController::OnTargetPerception);

	
}

void ACreatureController::BeginPlay()
{
	Super::BeginPlay();

	
	_enemy = Cast<AEnemy>(GetPawn());
	if(GetWorld()->GetFirstPlayerController()!= nullptr)
	{
		if(GetWorld()->GetFirstPlayerController()->GetPawn()!=nullptr)
		_targetPlayer = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
	
	//_aIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ACreatureController::OnTargetPerception);
	
	
}

void ACreatureController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(_targetPlayer != nullptr)
	{
		MoveToTarget();
	}
}

// void ACreatureController::OnTargetPerception(AActor* Actor, FAIStimulus Stimulus)
// {
// 	
// 	_targetPlayer = Cast<AMyCharacter>(Actor);
// 	
// 	if(_targetPlayer == nullptr && ActorHasTag("Player"))
// 	{
// 		_targetPlayer = Cast<AMyCharacter>(Actor);
// 		
// 		MoveToTarget();
// 	}
// }

void ACreatureController::MoveToTarget()
{
	if(_targetPlayer!=nullptr)
	{
		MoveToActor(_targetPlayer,-1,false,true,true,nullptr,true);
	}
}








