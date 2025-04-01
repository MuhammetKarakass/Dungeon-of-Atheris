// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{

	AbilitySystemComponent=CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet=CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

	
	NetUpdateFrequency=100.f;
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraPlayerState, PlayerLevel);
}

void AAuraPlayerState::OnRep_PlayerLevel(int32 OldLevel)
{
	
}
