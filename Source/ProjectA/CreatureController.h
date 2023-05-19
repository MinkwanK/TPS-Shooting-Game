// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController.h"
#include "Enemy.h"
#include "MyCharacter.h"
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
	

	// UFUNCTION()
	// void OnTargetPerception(AActor* Actor, FAIStimulus Stimulus); //인지된 하나의 타겟 액터만 리턴한다.

	void MoveToTarget();





protected:
	
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	AGameStageTimer* _gameStageTimer;
	AMyCharacter* _targetPlayer;
	AEnemy* _enemy;

		
	// class UAISenseConfig_Sight* _sight; //시야 감각을 위함.
	//
	// UPROPERTY(EditDefaultsOnly)
	// UAIPerceptionComponent* _aIPerceptionComponent;
	//
	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionStimuliSourceComponent* _aiPerceptionStimuliSourceComponent;
	

	


};
