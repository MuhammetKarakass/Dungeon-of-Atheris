// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HitReactAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FActiveGameplayEffectHandle UHitReactAbility::GetHitReactEffect()
{
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle SpecHandle=ASC->MakeOutgoingSpec(HitReact,GetAbilityLevel(),ASC->MakeEffectContext());
	return ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UHitReactAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
