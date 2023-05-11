// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "CreatureController.generated.h"

/**
	가상함수란?
	virtual은 가상함수이다. 부모클래스에서 상속받을 자식 클래스에서 재정의 할 것을 기대하고 정의한 함수입니다.
	가상함수를 사용하는 이유? 컴파일러가 함수 호출 코드는 컴파일 타임에 고정된 메모리 주소로 변환시킴 -> 정적바인딩
	컴파일 당시 호출될 함수의 번지가 이미 결정나서 원하는 함수의 동작이 안나올 수도 있음 따라서 가상함수를 사용하여 동적 바인딩을 한다.


 */
UCLASS()
class PROJECTA_API ACreatureController : public AAIController
{
	GENERATED_BODY()

public:
	ACreatureController();


	//FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return _blackboardComponent; }

	UPROPERTY(EditDefaultsOnly)
	class AEnemy* Agent;

	UFUNCTION()
	void OnTargetPerception(AActor* Actor, FAIStimulus Stimulus); //인지된 하나의 타겟 액터만 리턴한다.

	
	
	class UAISenseConfig_Sight* _sight; //시야 감각을 위함.


protected:
	

	
	virtual void BeginPlay() override;
	

	//우리가 컨트롤하고 싶은 액터가 스폰되면 이 함수가 실행된다. 이 함수안에서 블랙보드와 비헤비어트리를 초기화 합니다.
	
	virtual void OnPossess(APawn* InPawn) override;

public:	

	


public:
	
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* _behaviorTree;

	
	//BehaviorTree Reference
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTreeComponent* _behaviorTreeComponent;

	//Blackboard Component Reference
	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* _blackboardComponent;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* _aIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionStimuliSourceComponent* _aiPerceptionStimuliSourceComponent;
	
	//Blackboard Key
	UPROPERTY(EditDefaultsOnly)
	FName LocationToGoKey;
	


};
