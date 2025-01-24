// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UBaseAttributeSet* AuraAttributeSet=CastChecked<UBaseAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth(), AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UBaseAttributeSet* AuraAttributeSet=CastChecked<UBaseAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetHealthAttribute()).AddUObject(this,&UOverlayWidgetController::HealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this,&UOverlayWidgetController::MaxHealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetManaAttribute()).AddUObject(this,&UOverlayWidgetController::ManaChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this,&UOverlayWidgetController::MaxManaChange);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& TagContainer)
		{
			for (const FGameplayTag& Tag : TagContainer)
			{
				FGameplayTag MessageTag=FGameplayTag::RequestGameplayTag(FName("Message"));
				
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* WidgetRow=GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
				    MessageWidgetRowDelegate.Broadcast(*WidgetRow);
				}
			}
		});
}

void UOverlayWidgetController::HealthChange(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue,Data.OldValue);
}

void UOverlayWidgetController::MaxHealthChange(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChange(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}
void UOverlayWidgetController::MaxManaChange(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
