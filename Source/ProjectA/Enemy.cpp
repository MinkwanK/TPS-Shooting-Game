// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "CreatureController.h"
#include "DrawDebugHelpers.h"
#include "SmallTurret.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

/*
	Enemy의 행동로직
	Friendly 태그를 가진 액터를 만나면 target으로 저장하고 추격한다. (추격중인 target이 사망하면 새로운 target을 추격한다.)

	현재 방식 -> 인지된 터렛을 무작정 쫓아 때리는 방식
	새로운 방식 ? -> 맵안의 Player의 위치를 아는 적들. Player를 추격한다. 터렛과 일정 거리 범위로 가까워지면 터렛을 공격한다. 이게 디펜스 게임
	방식에 어울리고 구현도 더 쉽지 않을까?

	즉, 모든 Enemy들은 태어날 때부터 Player의 위치를 파악하고 있다. 무작정 Player를 추격하기 시작한다.

*/

AEnemy::AEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->MaxWalkSpeed = 300;
	
	_bCanAttack = false;
	_bDead = false;
	_bHit = false;

	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	switch (_MonsterTypeEnum)
	{
	case EMonsterType::Zombie :
		{
			_hp = 60;
			_damage = 10;
			GetCharacterMovement()->MaxWalkSpeed = 250;
			break;
		}

	case EMonsterType::Skeleton :
		{
			_hp = 120;
			_damage = 30;
			GetCharacterMovement()->MaxWalkSpeed = 500;
			break;
		}
	}
	if(GetWorld()->GetFirstPlayerController() != nullptr)
	{
		if(GetWorld()->GetFirstPlayerController()->GetPawn() != nullptr)
		{
			_targetObject = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		}
	}

	_gameStageTimer = Cast<AGameStageTimer>(UGameplayStatics::GetActorOfClass(GetWorld(),AGameStageTimer::StaticClass()));
	
	RHand = GetMesh()->GetBodyInstance("hand_r");
	PostInitializeComponents();


}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	animInstance = GetMesh()->GetAnimInstance();
	
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!_bDead)
	{
		if(FireRay() == true)
		{
			_bCanAttack =  true;
		}
		else
		{
			_bCanAttack = false;
		}
	
		if(_bCanAttack)
		{
			if( animInstance->Montage_GetIsStopped(_attackMontage))
			{
				animInstance->Montage_Play(_attackMontage);
			
				UE_LOG(LogTemp,Log,TEXT("Montage Play"));
			}
		}
		if(_bHit)
		{
			if(animInstance->Montage_GetIsStopped(_hitMontage))
			{
				animInstance->Montage_Play(_hitMontage);
			}
		}
	}

	if(_gameStageTimer!=nullptr)
	{
		if(_gameStageTimer->InGameEnum == EInGameState::GameRestState)
		{
			this->Destroy();
		}
	}


}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Enemy의 체력 차감 및 죽음 처리
void AEnemy::DecreaseHP(const int value)
{
	if(!_bDead)
	{
		_hp -= value;
		_bHit = true;
		if(_hp <= 0)
		{
			_bDead = true;
			UGameplayStatics::PlaySound2D(GetWorld(),_deadSound);
			ACreatureController* creatureController = Cast<ACreatureController>(this->Controller);
			creatureController->_aiPerceptionStimuliSourceComponent->UnregisterFromPerceptionSystem();
			this->AIControllerClass = nullptr;
			this->Controller->Destroy();
			this->SetActorEnableCollision(false);
		}
	}
	
	GetWorldTimerManager().SetTimer(_hitTimerHandle,this,&AEnemy::HitTimerFunc,_hitMontage->GetPlayLength(),false);

}

bool AEnemy::FireRay()
{
	FHitResult Hit;
	FVector Start = GetMesh()->GetSocketLocation("head");
	FVector End =  Start + GetMesh()->GetRightVector() * 100.0f;
		

	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit,Start,End,Channel,QueryParams);

	//DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,0.1f);

	if(Hit.GetActor()!=nullptr)
	{
		if(Hit.GetActor()->ActorHasTag("Player") || Hit.GetActor()->ActorHasTag("Turret"))
		{
			return true;
		}
	}

	return false;
}



void AEnemy::AttackStart()
{
	FTransform T = RHand->GetUnrealWorldTransform();
	_attackStartPos = T.GetLocation();
}

void AEnemy::AttackEnd()
{
	FTransform T = RHand->GetUnrealWorldTransform();
	
	TArray <struct FHitResult> hits;
	FHitResult hit;
	//sweep: 빗기
	GetWorld()->SweepMultiByChannel(
		hits,
		_attackStartPos,
		T.GetLocation(),
		T.GetRotation(),
		ECC_Visibility,
		FCollisionShape::MakeSphere(100.0f)
		
		);


	for(int i = 0 ; i < hits.Num();i++)
	{
		if(hits[i].GetActor()!=nullptr)
		{
			//터렛 공격 기능은 추후 개발
			// if(hits[i].GetActor()->ActorHasTag("Turret"))
			// {
			// 	ASmallTurret* SmallTurret = Cast<ASmallTurret>(hit.GetActor());
			//
			// 	if(SmallTurret != nullptr && SmallTurret->_hp > 0)
			// 	{
			// 		SmallTurret->DecreaseHP(20);
			//
			// 		if(SmallTurret->_hp <= 0)
			// 		{
			// 			_bCanAttack = false;
			// 		}
			// 	
			// 	}
			// 	
			// 	break;
			// }

			if(hits[i].GetActor()->ActorHasTag("Player"))
			{
				UE_LOG(LogTemp,Log,TEXT("Player got ray"));
				AMyCharacter* myCharacter = Cast<AMyCharacter>(hits[i].GetActor());

				if(myCharacter != nullptr && myCharacter->_hp > 0)
				{
					UE_LOG(LogTemp,Log,TEXT("Player got damage"));
					myCharacter->DecreaseHP(20);

					if(myCharacter<=0)
					{
						_bCanAttack = false;
					}
				}
				break;
			}
		}
	}

	//DrawDebugLine(GetWorld(), _attackStartPos, T.GetLocation(), FColor::Emerald, false, 3.0f, 0, 10);
}
