// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelCanChangeDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetAuraAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}

	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetAuraAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	}
	);
	
	if (GetAuraASC())
	{
		GetAuraASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);

		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	   (AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	   {
		   OnMaxManaChanged.Broadcast(Data.NewValue);
	   });
		GetAuraASC()->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	ULevelUpInfo* LevelUpInfo=GetAuraPS()->LevelUpInfo;
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

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	
	FAuraAbilityInfoNew LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_NONE;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegateNew.Broadcast(LastSlotInfo);

	FAuraAbilityInfoNew Info = AbilityInfoNew->FındAbilityInformationForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegateNew.Broadcast(Info);
}
