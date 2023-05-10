// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureController.h"
#include "Enemy.h"
#include "SmallTurret.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"

/*
	적, 아군 NPC 컨트롤러
	다른 Actor를 인지하고 이 컨트롤러로부터 조종 받는 Actor 또한 인지 받아야 한다.
	시각 Sense만을 사용

*/
ACreatureController::ACreatureController()
{
	_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
	_aIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	_aiPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPercetionStimulSorceComp"));

	_sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	_sight->SightRadius = 5000.0f;
	_sight->DetectionByAffiliation.bDetectNeutrals = true;


	_aIPerceptionComponent->ConfigureSense(*_sight);
	_aIPerceptionComponent->SetDominantSense(*_sight->GetSenseImplementation());

	
}

void ACreatureController::BeginPlay()
{
	Super::BeginPlay();

	//현재 소유한 캐릭터가 소유하고자 하는 클래스가 맞는지 확인한다.
	AEnemy* Enemy = Cast<AEnemy>(GetPawn());

	//비헤비어트리 실행 시작
	RunBehaviorTree(_behaviorTree);
	_behaviorTreeComponent->StartTree(*_behaviorTree);
}

void ACreatureController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//블랙보드가 비헤비어트리의 블랙보드를 사용하도록 설정
	Blackboard->InitializeBlackboard(*_behaviorTree->BlackboardAsset);

	_aIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ACreatureController::OnTargetPerception);
}


void ACreatureController::OnTargetPerception(AActor* Actor, FAIStimulus Stimulus)
{
	ASmallTurret* turret = Cast<ASmallTurret>(Actor);
	
	if(turret != nullptr)
	{
		UE_LOG(LogTemp,Display,TEXT("Enemy perception Success"));
		_blackboardComponent->SetValueAsObject("TargetObject",turret);
	}
}

