// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Sniper.generated.h"

//플레이어를 도와 적과 교전하는 스나이퍼 NPC
UCLASS()
class PROJECTA_API ASniper : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASniper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//인지된 하나의 TargetActor return
	UFUNCTION()
	void OnTargetPerception(AActor* Actor, FAIStimulus Stimulus);

	inline void SetFire() {_bCanFire = true;}

	FTimerHandle _fireTimerHandle;
	//시야 감각을 위함.
	class UAISenseConfig_Sight* _sight;
	
	AEnemy* _targetCreature;
	AGameStageTimer* _gameStageTimer;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectile> _projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _fireMontage;
	
	bool _bCanFire;
	void Aim();
	void Fire();
	void SmoothTurn(FRotator targetRotation,int InterpSeed);

};
