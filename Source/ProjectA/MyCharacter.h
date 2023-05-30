// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStageTimer.h"
#include "Projectile.h"
#include "TurretSpawnPoint.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyCharacter.generated.h"


//게임 내 플레이어의 스크립트. 사격, 이동, 재장전등을 수행
UCLASS()
class PROJECTA_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveRight(float value);
	void MoveForward(float value);
	void SmoothTurn(int initialValue);

	void DecreaseHP(int value);
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _reloadMontage;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* _gunEmptySound;
	
	ATurretSpawnPoint* _turretSpawnPoint;
	
	AGameStageTimer* _gameStageTimer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectile> _projectile;
	
	UPROPERTY(BlueprintReadOnly)
	int _ammo;
	
	int _maxAmmo;
	
	UPROPERTY(BlueprintReadWrite)
	int _ammoAmount;

	UPROPERTY(BlueprintReadOnly)
	int _hp;

	UPROPERTY(BlueprintReadWrite)
	int _money;

	UPROPERTY(BlueprintReadOnly)
	bool _bFire;

	UPROPERTY(BlueprintReadOnly)
	bool _bAim;

	UPROPERTY(BlueprintReadOnly)
	bool _bDead;

	UPROPERTY(BlueprintReadOnly)
	bool _bReload;
	
	UPROPERTY(BlueprintReadWrite)
	bool _bPaused;
	
	FVector muzzleVec;

	FTimerHandle _autoFireTimerHandle;
	FTimerHandle _reloadTimerHandle;
	FTimerHandle _aimTimerHandle;
	FTimerHandle _dieTimerHandle;
	
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* _camera;
	
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* _springArm;
	
	void FirePressed();
	void FireReleased();
	void Fire();
	void SpawnProjectile(FHitResult Hit);
	void Aim();
	void Relaod();
	void ReloadFinished();
	void DieSet() {_gameStageTimer->InGameEnum = EInGameState::GameEndState;  ;}
	
};
