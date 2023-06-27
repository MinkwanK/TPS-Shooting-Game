// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStageTimer.h"
#include "Projectile.h"
#include "TurretSpawnPoint.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyCharacter.generated.h"

//열거형 플레이어의 장비 상태 
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol UMETA (DisplayName = "Pistol"),
	Rifle UMETA (DisplayName = "Rifle"),
};

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

	//기본 이동 함수
	void MoveRight(float value);
	void MoveForward(float value);
	void SmoothTurn(int initialValue);

	//체력 차감 함수
	void DecreaseHP(int value);
	
	//사격 함수
	void FirePressed();
	void FireReleased();
	void Fire();
	void Throw(); //수류탄 투척
	//

	//스폰 함수
	void SpawnProjectile(FHitResult Hit);
	void SpawnGrenade();
	// 
	void Aim();
	void Relaod();
	void ReloadFinished();
	void DieSet() {_gameStageTimer->InGameEnum = EInGameState::GameEndState;  ;}

	//무기 교체 함수
	void SwapPistol();
	void SwapRifle();
	void SwapSet() {_bSwapping = false;} //무기를 교체 중인지를 설정하는 함수
	//
	void Shop();

	//애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _reloadMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _pistolReloadMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _equipWeaponMontage;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _ThrowMontage;
	//
	UPROPERTY(EditDefaultsOnly)
	USoundBase* _gunEmptySound;
	
	ATurretSpawnPoint* _turretSpawnPoint;
	
	AGameStageTimer* _gameStageTimer;

	//스폰되는 투사체 변수
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectile> _projectile;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectile> _pistolProjectile;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGrenade> _grenade;
	//

	
	UPROPERTY(BlueprintReadOnly)
	int _ammo;
	int _maxAmmo;
	
	UPROPERTY(BlueprintReadOnly)
	int _pistolAmmo;
	
	int _maxPistolAmmo;
	
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

	bool _bSwapping;

	UPROPERTY(BlueprintReadWrite)
	bool _bShop;

	int _damageDealt;
	
	FVector muzzleVec;

	//타이머핸들 
	FTimerHandle _autoFireTimerHandle;
	FTimerHandle _pistolFireTimerHandle;
	FTimerHandle _reloadTimerHandle;
	FTimerHandle _aimTimerHandle;
	FTimerHandle _dieTimerHandle;
	FTimerHandle _swapTimerHandle;
	FTimerHandle _throwTimerHandle;
	//
	
	UPROPERTY(BlueprintReadOnly)
	EWeaponType _playerWeaponType;
	
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* _camera;
	
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* _springArm;
	
	
};
