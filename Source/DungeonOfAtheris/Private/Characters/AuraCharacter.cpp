// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/BaseHUD.h"

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
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}


void AAuraCharacter::InitAbilityActorInfo()
{
	{
		AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
		if (!AuraPlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("AuraPlayerState is NULL!"));
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AuraPlayerState is already in use!"));
		}
		check(AuraPlayerState);
		AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
		Cast<UBaseAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
		AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
		AttributeSet = AuraPlayerState->GetAttributeSet();
		if (AAuraPlayerController* AuraController=Cast<AAuraPlayerController>(GetController()))
		{
			if (ABaseHUD* BaseHUD=Cast<ABaseHUD>(AuraController->GetHUD()))
			{
				BaseHUD->InitOverlay(AuraController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
			}
		}
		InitDefaultAttributes();
	}
}

int32 AAuraCharacter::GetLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetPlayerLevel();
}
