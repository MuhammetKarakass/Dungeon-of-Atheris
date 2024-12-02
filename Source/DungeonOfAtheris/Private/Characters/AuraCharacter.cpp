// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerState.h"

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
	GetCharacterMovement()->RotationRate=FRotator(0.0f, 400.0f, 0.0f);
	
    bUseControllerRotationPitch=false;
    bUseControllerRotationYaw=false;
    bUseControllerRotationRoll=false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	//init ability actor info for server
	Super::PossessedBy(NewController);
	InıtAbilityActorInfo();
}

void AAuraCharacter::OnRep_Controller()
{
	//init ability actor for client
	Super::OnRep_Controller();
	InıtAbilityActorInfo();
}

void AAuraCharacter::InıtAbilityActorInfo()
{
	AAuraPlayerState* PlayerState=GetPlayerState<AAuraPlayerState>();
	check(PlayerState);
	PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState,this);
	AbilitySystemComponent=PlayerState->GetAbilitySystemComponent();
	AttributeSet=PlayerState->GetAttributeSet();
}
