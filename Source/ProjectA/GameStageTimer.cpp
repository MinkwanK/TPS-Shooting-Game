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
	PrimaryActorTick.bCanEverTick = true;

}

//InGame Level이 열리면 게임 시작 준비 타이머 작동 
void AGameStageTimer::BeginPlay()
{
	Super::BeginPlay();

	if(GetWorld()->GetFirstPlayerController() != nullptr)
	{
		if ( GetWorld()->GetFirstPlayerController()->GetPawn() != nullptr )
		{
			_myCharacter = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

			GetWorldTimerManager().SetTimer(_stageReadyTimerHandle,this,&AGameStageTimer::StageReady,5.0f,false,5.0f);
		}
	}

	
	
	
}

// Called every frame
void AGameStageTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameStageTimer::StageReady()
{
	//스테이지 시작 준비 완료! 스테이지 카운트 타운은 timerhandle 값을 조회하여 표시 할 예정이다
	GetWorldTimerManager().SetTimer(_stageStartTimerHandle,this,&AGameStageTimer::StageStart,10.0f,false,10.0f);
	GetWorldTimerManager().ClearTimer(_stageReadyTimerHandle);
}

void AGameStageTimer::StageStart()
{
	GetWorldTimerManager().SetTimer(_stageRestTimerHandle,this,&AGameStageTimer::StageRest,5.0f,false,5.0f);
	GetWorldTimerManager().ClearTimer(_stageStartTimerHandle);
}

void AGameStageTimer::StageRest()
{
	GetWorldTimerManager().ClearTimer(_stageRestTimerHandle);
}




