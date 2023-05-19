// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStageTimer.generated.h"

UENUM(BlueprintType)
enum class EInGameState : uint8
{
	GameReadyState UMETA (DisplayName = "GameReadyState"),
	GameStartState UMETA (DisplayName = "GameStartState"),
	GameRestState  UMETA (DisplayName = "GameRestState"),
	GameEndState UMETA (DisplayName = "GameEndState"),
	
};


UCLASS()
class PROJECTA_API AGameStageTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameStageTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle _stageReadyTimerHandle;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle _stageStartTimerHandle;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle _stageRestTimerHandle;

	UPROPERTY(BlueprintReadOnly)
	int _stageNum;

	void StageReadyEnd();
	void StageStartEnd();
	void StageRestEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInGameState InGameEnum;

};
