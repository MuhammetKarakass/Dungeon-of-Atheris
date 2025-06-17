// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UBaseAttributeSet* AuraAttributeSet=CastChecked<UBaseAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{

	AAuraPlayerState* AuraPlayerState=CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	AuraPlayerState->OnLevelCanChangeDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
		});
	
	const UBaseAttributeSet* AuraAttributeSet=CastChecked<UBaseAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}

	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	}
	);
	
	if (UBaseAbilitySystemComponent* AuraASC=Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
	{

		if (AuraASC->bStartupAbilitiesGiven)
		{
			OnInıtialieStartupAbilities(AuraASC);
		}
		else
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInıtialieStartupAbilities);
		}
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	   (AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	   {
		   OnMaxManaChanged.Broadcast(Data.NewValue);
	   });
		AuraASC->EffectAssetTags.AddLambda(
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
}

void UOverlayWidgetController::OnInıtialieStartupAbilities(UBaseAbilitySystemComponent* AuraAbilitySystemComponent)
{
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfoNew Info = AbilityInfoNew->FındAbilityInformationForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegateNew.Broadcast(Info);
	});
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	AAuraPlayerState* AuraPlayerState=CastChecked<AAuraPlayerState>(PlayerState);
	ULevelUpInfo* LevelUpInfo=AuraPlayerState->LevelUpInfo;
	checkf(AuraPlayerState,TEXT("Fill out LevelUpInfo Blueprint in AuraPlayerState"));

	int32 Level=LevelUpInfo->FindLevelForXP(NewXP);
	int32 MaxLevel=LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level >= 0)
	{
		int32 LevelUpRequirement= LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		int32 PreviousLevelUpRequirement= LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		int32 DeltaLevelUpRequirement= LevelUpRequirement-PreviousLevelUpRequirement;
		int32 XPForThisLevel=NewXP-PreviousLevelUpRequirement;

		float XPBarPercent=static_cast<float>(XPForThisLevel)/static_cast<float>(DeltaLevelUpRequirement);

		OnPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}
