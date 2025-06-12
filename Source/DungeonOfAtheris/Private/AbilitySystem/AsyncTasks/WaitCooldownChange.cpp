// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent,
                                                                FGameplayTag InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange=NewObject<UWaitCooldownChange>();
	WaitCooldownChange->AbilitySystemComponent=InAbilitySystemComponent;
	WaitCooldownChange->CooldownTag=InCooldownTag;

	if (!IsValid(InAbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	
	InAbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CoolDownTagChanged);

	InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange,&UWaitCooldownChange::OnActiveEffectAdded);
	
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(AbilitySystemComponent)) return;
	AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CoolDownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount==0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveGameplayEffect)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag)||GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery EffectQuery=FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining=TargetASC->GetActiveEffectsTimeRemaining(EffectQuery);

		if (TimesRemaining.Num()>0)
		{
			float TimeRemaining=TimesRemaining[0];
			for (int i=0;i<TimesRemaining.Num();i++)
			{
				if (TimesRemaining[i]>TimeRemaining)
				{
					TimeRemaining=TimesRemaining[i];
				}
			}
			CooldownEnd.Broadcast(TimeRemaining);
		}
	}
	
	
}
