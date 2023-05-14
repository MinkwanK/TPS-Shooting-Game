// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

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
	UAnimInstance* animInstance;
	FBodyInstance * RHand;
	FVector _attackStartPos;

	AActor* _targetObject;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool _bCanAttack;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	bool _bDead;

	UPROPERTY(EditDefaultsOnly);
	UAnimMontage* _attackMontage;
	
	
	
	void DecreaseHP(int value);

	bool FireRay();
	
	UFUNCTION(BlueprintCallable)
	void AttackStart();

	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	



	
	virtual void PostInitializeComponents() override;
	



	

	
	
};
