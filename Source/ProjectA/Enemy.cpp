// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "CreatureController.h"
#include "DrawDebugHelpers.h"
#include "SmallTurret.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

/*
	Enemy의 행동로직
	플레이어 태그를 가진 적을 인지하면 추격 개시

	맵안의 Player의 위치를 아는 적들. Player를 추격한다. 
	즉, 모든 Enemy들은 태어날 때부터 Player의 위치를 파악하고 있다. 무작정 Player를 추격하기 시작한다.

	열거형에 따라 Enemy 종류에 따라 다른 속성 능력치를 준다.

*/

AEnemy::AEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->MaxWalkSpeed = 300;
	
	_bCanAttack = false;
	_bDead = false;
	_bHit = false;
	_gaveMoney = false;

	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//열거형에 따라 다른 능력치 부여
	switch (_MonsterTypeEnum)
	{
	case EMonsterType::Zombie :
		{
			_hp = 100;
			_damage = 10;
			GetCharacterMovement()->MaxWalkSpeed = 250;
			break;
		}

	case EMonsterType::Skeleton :
		{
			_hp = 150;
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


//머리에서 짧은 Ray를 계속 발사하여 Player가 맞으면 공겨 개시
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
			
				//UE_LOG(LogTemp,Log,TEXT("Montage Play"));
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
			AMyCharacter* player = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

			this->SetActorEnableCollision(false);
			if(player!=nullptr && !_gaveMoney)
			{
				player->_money += 10;
				_gaveMoney = true;
			}
			
			ACreatureController* creatureController = Cast<ACreatureController>(this->Controller);
			
			this->AIControllerClass = nullptr;
			this->Controller->Destroy();
		
			GetWorldTimerManager().SetTimer(_deadTimerHandle,this,&AEnemy::SetDead,8.0f);
		}
	}
	
	GetWorldTimerManager().SetTimer(_hitTimerHandle,this,&AEnemy::HitTimerFunc,_hitMontage->GetPlayLength(),false);

}

//실시간으로 레이를 발사하여 플레이어가 맞으면 전투 개시
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


//애님 노티파이를 이용한 전투용 함수
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
