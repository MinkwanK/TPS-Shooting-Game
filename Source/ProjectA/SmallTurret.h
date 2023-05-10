// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "SmallTurret.generated.h"

//플레이어가 설치할 수 있는 소구경 터렛
//범위 내의 적들에게 탄약을 발사한다. 탄약은 거리별 일정 오차범위내로 사격 실시한다.
//탄약이 다 떨어졌을 경우 Player가 접근하여 탄약을 보충해줘야한다.



UCLASS()
class PROJECTA_API ASmallTurret : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASmallTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void Aim();
	void Fire();

	//인지된 하나의 TargetActor return
	UFUNCTION()
	void OnTargetPerception(AActor* Actor, FAIStimulus Stimulus);
public:
	const int _maxAmmo = 30;
	int _ammo;
	int _hp;
	float fireRange;

	FVector _targetDirection;

	AEnemy* _enemy;

	//시야 감각을 위함.
	class UAISenseConfig_Sight* _sight;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* _aiPerceptionComp;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionStimuliSourceComponent* _aiPerceptionStimulSourceComp;
	
	

};
