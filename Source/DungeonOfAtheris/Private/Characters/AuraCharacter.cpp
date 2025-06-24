// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
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

	LevelUpNiagaraComponent=CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate=false;
	
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

	CharacterClass=ECharacterClass::Elementalist;
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

int32 AAuraCharacter::GetLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MultiCastLevelUpParticles_Implementation();
}

void AAuraCharacter::MultiCastLevelUpParticles_Implementation() const
{
	if (LevelUpNiagaraComponent)
	{
		FVector CameraLocation=CameraComp->GetComponentLocation();
		FVector NiagaraLocation=LevelUpNiagaraComponent->GetComponentLocation();

		FRotator ToCameraRotation=(CameraLocation-NiagaraLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
		// LevelUpNiagaraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

int32 AAuraCharacter::GetXP_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InPlayerLevel].SpellPointAward;
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InPlayerLevel].AttributePointAward;
}

int32 AAuraCharacter::GetAttributePoints_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}


void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InPlayerLevel);
	if (UBaseAbilitySystemComponent* ASC=Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}