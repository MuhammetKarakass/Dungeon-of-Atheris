// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AAuraCharacter::AAuraCharacter()
{
	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength=300;
	SpringArmComp->SetRelativeRotation(FRotator(0.0f, -45.0f, 0.0f));

	CameraComp=CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);

	
	SpringArmComp->bUsePawnControlRotation=false;
	SpringArmComp->bEnableCameraLag=true;

	SpringArmComp->bInheritPitch=false;
	SpringArmComp->bInheritRoll=false;
	SpringArmComp->bInheritYaw=false; 
	
	GetCharacterMovement()->bOrientRotationToMovement=true;
	GetCharacterMovement()->bConstrainToPlane=true;
	GetCharacterMovement()->bSnapToPlaneAtStart=true;
	
    bUseControllerRotationPitch=false;
    bUseControllerRotationYaw=false;
    bUseControllerRotationRoll=false;
}
