// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Passive/AuraHaloOfProtection.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

FString UAuraHaloOfProtection::GetDescription(const int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 DamageReduce = DamageReductionCurve.GetValueAtLevel(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>Halo of Protection</>\n\n"

		// Details
		"<Small>Skill Level:</><Level>%d</>\n"
		"<Small>Mana Cost:</><ManaCost>%.1f</>\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Generates</><Damage> 1 </><Default>protective halo around the character, "
		"reducing all types of incoming damage by</><ManaCost> %d%% </>\n\n"),

						   Level, ManaCost, Cooldown, DamageReduce);
}

FString UAuraHaloOfProtection::GetNextLevelDescription(const int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 DamageReduce = DamageReductionCurve.GetValueAtLevel(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>Next Level</>\n\n"

		// Details
		"<Small>Skill Level:</><Level>%d</>\n"
		"<Small>Mana Cost:</><ManaCost>%.1f</>\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Generates</><Damage> 1 </><Default>protective halo around the character, "
		"reducing all types of incoming damage by</><ManaCost> %d%% </>\n\n"),

						   Level, ManaCost, Cooldown, DamageReduce);
}

void UAuraHaloOfProtection::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
											const FGameplayAbilityActorInfo* ActorInfo,
											const FGameplayAbilityActivationInfo ActivationInfo,
											const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UBaseAbilitySystemComponent* AuraASC = Cast<UBaseAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->AddLooseGameplayTag(FBaseGameplayTags::Get().Abilities_Passive_HaloOfProtection);
	}
}

void UAuraHaloOfProtection::EndAbility(const FGameplayAbilitySpecHandle Handle,
									   const FGameplayAbilityActorInfo* ActorInfo,
									   const FGameplayAbilityActivationInfo ActivationInfo,
									   bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UBaseAbilitySystemComponent* AuraASC = Cast<UBaseAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->RemoveLooseGameplayTag(FBaseGameplayTags::Get().Abilities_Passive_HaloOfProtection);
	}
}