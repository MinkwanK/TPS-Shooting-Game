// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStageTimer.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

//플레이어와 교전을 벌이는 적의 스크립트


//열거형을 통해 각 적들의 종류 판단.
UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Zombie UMETA (DisplayName = "Zombie"),
	Skeleton UMETA (DisplayName = "Skeleton"),
};
UCLASS()
class PROJECTA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	

	int _hp;
	int _damage;

	bool _gaveMoney;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool _bCanAttack;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	bool _bDead;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	bool _bHit;

	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMonsterType  _MonsterTypeEnum;

	FTimerHandle _deadTimerHandle;
	UAnimInstance* animInstance;
	FBodyInstance * RHand;
	FVector _attackStartPos;

	

	FTimerHandle _hitTimerHandle;

	AActor* _targetObject;
	AGameStageTimer* _gameStageTimer;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* _deadSound;

	

	UPROPERTY(EditDefaultsOnly);
	UAnimMontage* _attackMontage;

	UPROPERTY(EditDefaultsOnly);
	UAnimMontage* _hitMontage;
	
	void DecreaseHP(int value);

	bool FireRay();
	
	UFUNCTION(BlueprintCallable)
	void AttackStart();
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	inline void HitTimerFunc() {_bHit = false;}
	
	virtual void PostInitializeComponents() override;

	void SetDead() {this->Destroy();}
	
	
};
