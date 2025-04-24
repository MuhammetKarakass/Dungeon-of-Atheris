// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"

#include "BaseGameplayTags.h"


void UBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UBaseAbilitySystemComponent::EffectApplied);
	const  FBaseGameplayTags& GameplayTags=FBaseGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString::Printf(TEXT("%s"),*GameplayTags.Attribute_Secondary_Armor.ToString()));
}

void UBaseAbilitySystemComponent::AddCharacterAbilites(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilites)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass:StartupAbilites)
	{
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UBaseAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
}
