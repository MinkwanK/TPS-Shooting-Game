// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStageTimer.h"

/*
게임이 시작 됐을 때, 각 스테이지의 남은 시간을 재는 타이머
각 스테이지마다 정해진 시간동안 플레이어가 몰려오는 적을 막아 살아남으면 다음 스테이지로 넘어간다.
플레이어는 각 스테이지가 끝나고 남은 시간에 무기를 정비하거나 터렛을 구매해 설치할 수 있다.

*/
// Sets default values
AGameStageTimer::AGameStageTimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InGameEnum = EInGameState::GameReadyState;
	_stageNum = 2;

}

//InGame Level이 열리면 게임 시작 준비 타이머 작동 
void AGameStageTimer::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Log,TEXT("Game Ready Timer Start"));
	GetWorldTimerManager().SetTimer(_stageReadyTimerHandle,this,&AGameStageTimer::StageReadyEnd,1.0f,false);
		
}



// Called every frame
void AGameStageTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}


void AGameStageTimer::StageReadyEnd()
{
	
	UE_LOG(LogTemp,Log,TEXT("Game Start Timer Start"));
	InGameEnum = EInGameState::GameStartState;
	//스테이지 시작 준비 완료! 스테이지 카운트 타운은 timerhandle 값을 조회하여 표시 할 예정이다

	if(GetWorldTimerManager().IsTimerActive(_stageReadyTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(_stageReadyTimerHandle);
	}
	GetWorldTimerManager().SetTimer(_stageStartTimerHandle,this,&AGameStageTimer::StageStartEnd,60.0f,false);
	
	
}

void AGameStageTimer::StageStartEnd()
{
	
	UE_LOG(LogTemp,Log,TEXT("Game Rest Timer Start"));

	GetWorldTimerManager().ClearTimer(_stageStartTimerHandle);

	if(_stageNum < 3)
	{
		InGameEnum = EInGameState::GameRestState;
		GetWorldTimerManager().SetTimer(_stageRestTimerHandle,this,&AGameStageTimer::StageRestEnd,5.0f,false);
	}
	else
	{
		InGameEnum = EInGameState::GameEndState;
	}
	
	
	
}

//정해진 최종 스테이지까지 계속해서 반복한다.
void AGameStageTimer::StageRestEnd()
{
	GetWorldTimerManager().ClearTimer(_stageRestTimerHandle);
	
	InGameEnum = EInGameState::GameStartState;

	GetWorldTimerManager().SetTimer(_stageStartTimerHandle,this,&AGameStageTimer::StageStartEnd,60.0f,false);
	_stageNum++;
	
}




