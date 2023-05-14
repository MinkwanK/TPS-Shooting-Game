// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretSpawnPoint.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyCharacter.generated.h"

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

	void DecreaseHP(int value);
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _reloadMontage;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* _gunSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* _reloadSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* _gunEmptySound;


	ATurretSpawnPoint* _turretSpawnPoint;
	
	UPROPERTY(BlueprintReadOnly)
	int _ammo;
	
	int _maxAmmo;
	
	UPROPERTY(BlueprintReadOnly)
	int _ammoAmount;

	UPROPERTY(BlueprintReadOnly)
	int _hp;

	UPROPERTY(BlueprintReadOnly)
	int _money;

	UPROPERTY(BlueprintReadOnly)
	bool _bFire;

	UPROPERTY(BlueprintReadOnly)
	bool _bAim;

	UPROPERTY(BlueprintReadOnly)
	bool _bDead;

	UPROPERTY(BlueprintReadOnly)
	bool _bReload;

	UPROPERTY(BlueprintReadOnly)
	bool _bOverlapTurretItem;

	UPROPERTY(BlueprintReadOnly)
	bool _bOverlapAmmoBox;
	
	FTimerHandle _autoFireTimerHandle;
	FTimerHandle _reloadTimerHandle;
	
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* _camera;
	
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* _springArm;
	
	void FirePressed();
	void FireReleased();
	void Fire();
	void Aim();
	void Relaod();
	void ReloadFinished();

	void Interact();

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;
	



};
