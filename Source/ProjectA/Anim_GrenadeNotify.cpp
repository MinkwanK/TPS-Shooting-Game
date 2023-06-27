// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_GrenadeNotify.h"

#include "MyCharacter.h"

void UAnim_GrenadeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	

	AMyCharacter* myCharacter = Cast<AMyCharacter>(MeshComp->GetOwner());

	if(myCharacter!=nullptr)
	{
		myCharacter->SpawnGrenade();
	}
}
