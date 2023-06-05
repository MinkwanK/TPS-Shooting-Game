// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "DrawDebugHelpers.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

/*
 *게임 무대의 주인공인 플레이어
 *
 무기 변경이 가능하다. 1번 권총 2번 소총
*/
// 생성자를 통해 플레이어의 탄약, 카메라, 체력 세팅
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
	_bPaused = false;

	_playerWeaponType = EWeaponType::Pistol;
	_hp = 100;
	_ammo = 30;
	_maxAmmo = 30;
	_money = 120;
	_ammoAmount = 240;
	_pistolAmmo = 12;
	_maxPistolAmmo = 12;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	_gameStageTimer = Cast<AGameStageTimer>(UGameplayStatics::GetActorOfClass(GetWorld(),AGameStageTimer::StaticClass()));
	
	
}

//TPS 조준 시스템의 오차를 위해 가까이 있는 적은 총구에서 레이를 발사하여 전투 (수정필요)
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

	PlayerInputComponent->BindAction("Swap Pistol",IE_Pressed,this,&AMyCharacter::SwapPistol);
	PlayerInputComponent->BindAction("Swap Rifle",IE_Pressed,this,&AMyCharacter::SwapRifle);
	

	PlayerInputComponent->BindAxis("Move Right",this,&AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Move Forward",this,&AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn",this,&AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up",this,&AMyCharacter::AddControllerPitchInput);
	
		
}

void AMyCharacter::MoveRight(float Value)
{
	if(Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0,Controller->GetControlRotation().Yaw,0); //카메라가 바라보는 방향을 기준으로 캐릭터가 이동하기 위해서 yaw값을 구한다.
		const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation); //YawRotation을 가리키는 벡터를 구한다.
		SmoothTurn(3);
		AddMovementInput(Direction,Value);
	}
	
}

void AMyCharacter::MoveForward(float Value)
{
	
	if(Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0,Controller->GetControlRotation().Yaw,0); //카메라가 바라보는 방향을 기준으로 캐릭터가 이동하기 위해서 yaw값을 구한다.
		const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation); //YawRotation을 가리키는 벡터를 구한다.
		SmoothTurn(3);
		AddMovementInput(Direction,Value);


	}
	
}


//조준을 해야 사격이 가능하다. 조준 시 속도 저하
//조준 중에는 카메라 회전에 따라 플레이어의 rotation도 회전
void AMyCharacter::Aim()
{
	
	if(_bAim)
	{
		_bAim = false;
		bUseControllerRotationYaw = false;
		_springArm->TargetArmLength = 300;
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	else if(!_bAim)
	{
		_bAim = true;
		bUseControllerRotationYaw = true;
		_springArm->TargetArmLength = 100;
		_springArm->SocketOffset = FVector(0,-100,0);
		GetCharacterMovement()->MaxWalkSpeed = 200;
	}
	

}

//부드러운 캐릭터의 회전을 위한 함수이다.
//카메라는 처음에 자유시점 카메라로 있다가 플레이어가 이동을 시작하면 카메라방향으로 플레이어가 이동한다.
void AMyCharacter::SmoothTurn(int initialValue)
{
	//카메라가 바라보는 방향으로 자연스럽게 회전할 수 있도록 보간.
	const FRotator targetRotation = FRotator(0,_camera->GetComponentRotation().Yaw,0);
	const FRotator resultRotation = FMath::RInterpTo(GetActorRotation(),targetRotation,GetWorld()->GetDeltaSeconds(),initialValue);
		
	SetActorRotation(resultRotation);
}

//연사를 위한 함수(타이머사용)
void AMyCharacter::FirePressed()
{
	if(_bAim && !_bSwapping )
	{
		switch (_playerWeaponType)
		{
			case EWeaponType::Pistol:
			{
				if(_pistolAmmo > 0 )
				{
					Fire();
					_bFire = true;
					GetWorldTimerManager().SetTimer(_autoFireTimerHandle,this,&AMyCharacter::Fire,1.0,true,0.2);
				}
				else
				{
					UGameplayStatics::PlaySoundAtLocation(this->GetWorld(),_gunEmptySound,GetActorLocation());
				}
				break;	
			}
			case EWeaponType::Rifle:
			{
				if(_ammo > 0 )
				{
					Fire();
					_bFire = true;
					GetWorldTimerManager().SetTimer(_autoFireTimerHandle,this,&AMyCharacter::Fire,0.15,true,0.15);
				}
				else
				{
					UGameplayStatics::PlaySoundAtLocation(this->GetWorld(),_gunEmptySound,GetActorLocation());
				}
				break;
			}
		}
		
	}
}

//사격 입력을 멈췄을 때
void AMyCharacter::FireReleased()
{
	_bFire = false;
	GetWorldTimerManager().ClearTimer(_autoFireTimerHandle);
}

//플레이어의 사격 수행 , Ray를 발사한 뒤에 투사체 생성
void AMyCharacter::Fire()
{
	switch(_playerWeaponType)
	{
	case EWeaponType::Pistol:
		{
			if(_pistolAmmo > 0)
			{
				FHitResult Hit;
	
				FVector RayStart  = _camera->GetComponentLocation();
				FVector RayEnd = RayStart + _camera->GetForwardVector() * 5000.0f;
	
				ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
				FCollisionQueryParams QueryParams;
	
				QueryParams.AddIgnoredActor(this);
	
				GetWorld()->LineTraceSingleByChannel(Hit,RayStart,RayEnd,Channel,QueryParams);
				//DrawDebugLine(GetWorld(),RayStart,RayEnd,FColor::Red,false,5.0f);
				SpawnProjectile(Hit);
			}
			else
			{
				_bFire = false;
			}
			
			break;
		}
	case EWeaponType::Rifle:
		{
			if(_ammo > 0)
			{
				FHitResult Hit;
	
				FVector RayStart  = _camera->GetComponentLocation();
				FVector RayEnd = RayStart + _camera->GetForwardVector() * 5000.0f;
	
				ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
				FCollisionQueryParams QueryParams;
	
				QueryParams.AddIgnoredActor(this);
	
				GetWorld()->LineTraceSingleByChannel(Hit,RayStart,RayEnd,Channel,QueryParams);
				//DrawDebugLine(GetWorld(),RayStart,RayEnd,FColor::Red,false,5.0f);
				SpawnProjectile(Hit);
			}
			else
			{
				_bFire = false;
			}
			break;
		}
	}
	
	
}

void AMyCharacter::SpawnProjectile(FHitResult Hit)
{
	switch(_playerWeaponType)
	{
	case EWeaponType::Pistol:
		{
			if(_pistolAmmo > 0 )
			{
				_pistolAmmo -= 1;
	
				//총구의 위치 구하기
				FVector ProjectileSpawnLocation = GetMesh()->GetChildComponent(0)->GetSocketLocation("b_gun_muzzleflash");
				FRotator ProjectileSpawnRotation;

				//Ray가 Hit한 Actor가 있을 때는 그 Actor를 바라보는 방향 각도를 이용하여 스폰.
				if(Hit.GetActor() != nullptr)
				{
					ProjectileSpawnRotation = UKismetMathLibrary::FindLookAtRotation(ProjectileSpawnLocation,Hit.Location);
				}
				//Ray가 Hit한 Actor가 없을 경우에는 카메라의 Rotation 구하기
				else
				{
					ProjectileSpawnRotation = _camera->GetComponentRotation();
				}
	
				FTransform ProjectileSpawnTransform = UKismetMathLibrary::MakeTransform(ProjectileSpawnLocation,ProjectileSpawnRotation,FVector(1,1,1));
	
	
				if(_projectile)
				{
		
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
		
					AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(_pistolProjectile,ProjectileSpawnTransform,SpawnParams);
					projectile->SetOwner(this);
					if(projectile)
					{
						//투사체가 자기 자신은 무시하도록 하기.
						projectile->_CollisionComp->MoveIgnoreActors.Add(SpawnParams.Owner);
					}
				}
			}
			break;
		}
	case EWeaponType::Rifle:
		{
			if(_ammo>0)
			{
				_ammo -= 1;
	
				//총구의 위치 구하기
				FVector ProjectileSpawnLocation = GetMesh()->GetChildComponent(0)->GetSocketLocation("b_gun_muzzleflash");
				FRotator ProjectileSpawnRotation;

				//Ray가 Hit한 Actor가 있을 때는 그 Actor를 바라보는 방향 각도를 이용하여 스폰.
				if(Hit.GetActor() != nullptr)
				{
					ProjectileSpawnRotation = UKismetMathLibrary::FindLookAtRotation(ProjectileSpawnLocation,Hit.Location);
				}
				//Ray가 Hit한 Actor가 없을 경우에는 카메라의 Rotation 구하기
				else
				{
					ProjectileSpawnRotation = _camera->GetComponentRotation();
				}
	
				FTransform ProjectileSpawnTransform = UKismetMathLibrary::MakeTransform(ProjectileSpawnLocation,ProjectileSpawnRotation,FVector(1,1,1));
	
	
				if(_projectile)
				{
		
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
		
					AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(_projectile,ProjectileSpawnTransform,SpawnParams);
					projectile->SetOwner(this);
					if(projectile)
					{
						//투사체가 자기 자신은 무시하도록 하기.
						projectile->_CollisionComp->MoveIgnoreActors.Add(SpawnParams.Owner);
					}
				}
			}
			break;
		}
	}
	
}


//재장전
void AMyCharacter::Relaod()
{
	switch(_playerWeaponType)
	{
	case EWeaponType::Pistol:
		{
			if(_pistolAmmo < 12 && _bReload == false )
			{
				_bReload = true;
				PlayAnimMontage(_pistolReloadMontage);
				GetWorldTimerManager().SetTimer(_reloadTimerHandle,this,&AMyCharacter::ReloadFinished,2.16f,false);
			}
			break;
		}
	case EWeaponType::Rifle:
		{
			if(_ammo<30 && _ammoAmount > 0 && _bReload == false )
			{
				_bReload = true;
				PlayAnimMontage(_reloadMontage);
				GetWorldTimerManager().SetTimer(_reloadTimerHandle,this,&AMyCharacter::ReloadFinished,2.16f,false);
			}
			break;
		}
	}
	
}

//몽타쥬 플레이 시간이 지난 뒤에 이 함수를 실행하여 장전 완료
void AMyCharacter::ReloadFinished()
{
	switch(_playerWeaponType)
	{
	case EWeaponType::Pistol:
		{
			_pistolAmmo = 12;
			_bReload = false;
			break;
		}
	case EWeaponType::Rifle:
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
			break;
		}
	}
	

	

}

void AMyCharacter::SwapPistol()
{
	_playerWeaponType = EWeaponType::Pistol;
	PlayAnimMontage(_equipWeaponMontage);
	_bAim = false;
	_bSwapping = true;
	GetWorldTimerManager().SetTimer(_swapTimerHandle,this,&AMyCharacter::SwapSet,1.7f);
	
}

void AMyCharacter::SwapRifle()
{
	_playerWeaponType = EWeaponType::Rifle;
	PlayAnimMontage(_equipWeaponMontage);
	_bAim = false;
	_bSwapping = true;
	GetWorldTimerManager().SetTimer(_swapTimerHandle,this,&AMyCharacter::SwapSet,1.7f);
}

//플레이어의 체력 차감
void AMyCharacter::DecreaseHP(int value)
{
	_hp -= value;
	UE_LOG(LogTemp,Log,TEXT("Player HP:: %d"),_hp);
	
	if(_hp <=0)
	{
		_bDead = true;
		_gameStageTimer->InGameEnum = EInGameState::GameEndState;
		SetActorEnableCollision(false);
		//this->Controller->Destroy();
	}
}




