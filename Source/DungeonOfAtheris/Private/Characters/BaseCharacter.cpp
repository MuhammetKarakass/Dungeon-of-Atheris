// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::InitAbilityActorInfo()
{
}


void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<class UGameplayEffect> Effect, float level) const
{
	check(AbilitySystemComponent);
	check(Effect);

	FGameplayEffectContextHandle ContextHandle=GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle=GetAbilitySystemComponent()->MakeOutgoingSpec(Effect,level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void ABaseCharacter::InitDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.0f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.0f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.0f);
}

void ABaseCharacter::AddCharacterAbilities()
{
	if (!HasAuthority()) return;
	if (UBaseAbilitySystemComponent* ABC=Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
	{
		ABC->AddCharacterAbilites(Abilities);
	}
	
}
