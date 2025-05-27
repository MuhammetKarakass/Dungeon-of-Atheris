// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "BaseAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/BaseHUD.h"
#include "UI/WidgetController/BaseWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if(APlayerController* PC=UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (ABaseHUD* HUD=Cast<ABaseHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS=PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAttributeSet* AS=PS->GetAttributeSet();

			const FWidgetControllerParams WidgetParams=FWidgetControllerParams(PC,PS,ASC,AS);

			return HUD->GetOverlayWidgetController(WidgetParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if(APlayerController* PC=UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (ABaseHUD* HUD=Cast<ABaseHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS=PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAttributeSet* AS=PS->GetAttributeSet();

			const FWidgetControllerParams WidgetParams=FWidgetControllerParams(PC,PS,ASC,AS);

			return HUD->GetAttributeMenuWidgetController(WidgetParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC)
{
	AActor* Actor=ASC->GetAvatarActor();
	// AAuraGameModeBase* AuraGameMode=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	// if (AuraGameMode==nullptr) return;

	FCharacterClassDefaultInfo ClassDefaultInfo=GetCharacterClassInfo(WorldContextObject)->GetDefaultClassInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributeContextHandle=ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(Actor);
	FGameplayEffectSpecHandle PrimaryAttributesSpecHandle=ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,level,PrimaryAttributeContextHandle );
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributeContextHandle=ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(Actor);
	FGameplayEffectSpecHandle SecondaryAttributesSpecHandle=ASC->MakeOutgoingSpec(GetCharacterClassInfo(WorldContextObject)->SecondaryAttributes,level, SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributeContextHandle=ASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(Actor);
	FGameplayEffectSpecHandle VitalAttributesSpecHandle=ASC->MakeOutgoingSpec(GetCharacterClassInfo(WorldContextObject)->VitalAttributes,level, VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	for (auto AbilityClass:CharacterClassInfo->Abilities)
	{
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* GameMode=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode==nullptr) return nullptr;
	return GameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* AuraEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}

	else
	{
		return false;
	}
	
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* AuraEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	else
	{
		return  false;
	}
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FBaseGameplayEffectContext* AuraEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FBaseGameplayEffectContext* AuraEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}