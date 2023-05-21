// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "DrawDebugHelpers.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	
	 _springArm->SetupAttachment(RootComponent);
	 _camera->SetupAttachment(_springArm,USpringArmComponent::SocketName);


	_springArm -> TargetArmLength  = 300;
	_springArm->SocketOffset = FVector(0,-100,0);
	_camera->SetWorldRotation(FRotator(0,0,0));

	_turretSpawnPoint = nullptr;
	
	_bDead = false;
	_bAim = false;
	_bFire = false;
	_bOverlapTurretItem = false;
	_bOverlapAmmoBox = false;
	_bPaused = false;
	
	_hp = 100;
	_ammo = 30;
	_maxAmmo = 30;
	_money = 120;
	_ammoAmount = 60;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	_gameStageTimer = Cast<AGameStageTimer>(UGameplayStatics::GetActorOfClass(GetWorld(),AGameStageTimer::StaticClass()));
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMyCharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&AMyCharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AMyCharacter::FirePressed);
	PlayerInputComponent->BindAction("Fire",IE_Released,this,&AMyCharacter::AMyCharacter::FireReleased);

	PlayerInputComponent->BindAction("Aim",IE_Pressed,this,&AMyCharacter::Aim);
	PlayerInputComponent->BindAction("Aim",IE_Released,this,&AMyCharacter::Aim);

	PlayerInputComponent->BindAction("Reload",IE_Pressed,this,&AMyCharacter::Relaod);

	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&AMyCharacter::Interact);



	PlayerInputComponent->BindAxis("Move Right",this,&AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Move Forward",this,&AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn",this,&AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up",this,&AMyCharacter::AddControllerPitchInput);
	
		
}

void AMyCharacter::MoveRight(float Value)
{
	if(_gameStageTimer != nullptr)
	{
		if(_gameStageTimer->InGameEnum != EInGameState::GameReadyState )
			if(Controller != nullptr && Value != 0.0f)
			{
				const FRotator YawRotation(0,Controller->GetControlRotation().Yaw,0); //카메라가 바라보는 방향을 기준으로 캐릭터가 이동하기 위해서 yaw값을 구한다.
				const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation); //YawRotation을 가리키는 벡터를 구한다.
				AddMovementInput(Direction,Value);
			}
	}
}

void AMyCharacter::MoveForward(float Value)
{
	if(_gameStageTimer != nullptr)
	{
		if(_gameStageTimer->InGameEnum != EInGameState::GameReadyState )
			if(Controller != nullptr && Value != 0.0f)
			{
				const FRotator YawRotation(0,Controller->GetControlRotation().Yaw,0); //카메라가 바라보는 방향을 기준으로 캐릭터가 이동하기 위해서 yaw값을 구한다.
				const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation); //YawRotation을 가리키는 벡터를 구한다.
				AddMovementInput(Direction,Value);


			}
	}
}


//조준을 해야 사격이 가능하다. 조준 시 속도 저하
void AMyCharacter::Aim()
{
	if(_gameStageTimer != nullptr)
	{
		if(_gameStageTimer->InGameEnum != EInGameState::GameReadyState )
		{
			if(_bAim)
			{
				_bAim = false;
				_springArm->TargetArmLength = 300;
				GetCharacterMovement()->MaxWalkSpeed = 600;
			}
			else
			{
				_bAim = true;
				_springArm->TargetArmLength = 100;
				GetCharacterMovement()->MaxWalkSpeed = 150;
			}
		}
	}

}


//연사를 위한 함수
void AMyCharacter::FirePressed()
{
	if(_gameStageTimer != nullptr)
	{
		if(_gameStageTimer->InGameEnum != EInGameState::GameReadyState )
		{
			if(_bAim)
			{
				if(_ammo > 0 )
				{
					Fire();
					_bFire = true;
					GetWorldTimerManager().SetTimer(_autoFireTimerHandle,this,&AMyCharacter::Fire,0.10,true,0.10);
				}
				else
				{
					UGameplayStatics::PlaySoundAtLocation(this->GetWorld(),_gunEmptySound,GetActorLocation());
				}
		
			}
		}
	}
	
}
void AMyCharacter::FireReleased()
{
	_bFire = false;
	GetWorldTimerManager().ClearTimer(_autoFireTimerHandle);
	
}


//사격
void AMyCharacter::Fire()
{
	
	if(_ammo > 0  && _bReload == false)
	{
		UGameplayStatics::PlaySoundAtLocation(this->GetWorld(),_gunSound,GetActorLocation());
		_ammo -= 1;
		UE_LOG(LogTemp,Log,TEXT("Ammo :: %d"),_ammo);
		FHitResult Hit;


		FVector Start = _camera->GetComponentToWorld().GetLocation();
		FVector End =  Start + _camera->GetForwardVector() * 50000.0f;


		ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
		FCollisionQueryParams QueryParams;

		QueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(Hit,Start,End,Channel,QueryParams);

		//DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,1.0f);

		if(Hit.GetActor() != nullptr)
		{
			if(Hit.GetActor()->ActorHasTag("Enemy"))
			{
				AEnemy* hitEnemy = Cast<AEnemy>(Hit.GetActor());
				hitEnemy->DecreaseHP(20);

				if(hitEnemy->_hp <=0)
				{
					_money += 10;
				}
			}
		
		}
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this->GetWorld(),_gunEmptySound,GetActorLocation());
		_bFire = false;
	}
	

}


//재장전
void AMyCharacter::Relaod()
{
	if(_gameStageTimer != nullptr)
	{
		if(_gameStageTimer->InGameEnum != EInGameState::GameReadyState )
		{
			if(_ammo<30 && _ammoAmount > 0 && _bReload == false )
			{
				_bReload = true;
				UGameplayStatics::PlaySoundAtLocation(this->GetWorld(),_reloadSound,GetActorLocation());
				GetWorldTimerManager().SetTimer(_reloadTimerHandle,this,&AMyCharacter::ReloadFinished,2.16f,false);
			}
		}
	}
	
	
}

//몽타쥬 플레이 시간이 지난 뒤에 이 함수를 실행하여 장전 완료
void AMyCharacter::ReloadFinished()
{
	
	if(_ammoAmount > 30)
	{
		_ammoAmount -= _maxAmmo -_ammo;
		_ammo = 30;
		_bReload = false;
	}
	else
	{
		_ammo = _ammoAmount;
		_ammoAmount = 0;
		_bReload = false;
		
	}

	

}


void AMyCharacter::DecreaseHP(int value)
{
	_hp -= value;
	UE_LOG(LogTemp,Log,TEXT("Player HP:: %d"),_hp);
	
	if(_hp <=0)
	{
		_bDead = true;
		_gameStageTimer->InGameEnum = EInGameState::GameEndState;
		//this->Controller->Destroy();
	}
}

void AMyCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(OtherActor->ActorHasTag("TurretSpawn"))
	{
		_bOverlapTurretItem = true;
		_turretSpawnPoint = Cast<ATurretSpawnPoint>(OtherActor);
	}
	
	else if(OtherActor->ActorHasTag("AmmoBox"))
    {
    	_bOverlapAmmoBox = true;
    }
}

void AMyCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	if(OtherActor->ActorHasTag("TurretSpawn"))
	{
		_bOverlapTurretItem = false;
		_turretSpawnPoint = nullptr;
	}

	if(OtherActor->ActorHasTag("AmmoBox"))
	{
		_bOverlapAmmoBox = false;
	}
}

void AMyCharacter::Interact()
{
	if(_bOverlapTurretItem == true)
	{
		if(_money >= 120)
		{
			if(_turretSpawnPoint!=nullptr)
			{
				_turretSpawnPoint->SpawnTurret();
				_money -= 120;
			}
		}
	}

	else if(_bOverlapAmmoBox==true)
	{
		if(_money >= 30)
		{
			_ammoAmount += 30;
			_money -= 30;
		}
	}
}


