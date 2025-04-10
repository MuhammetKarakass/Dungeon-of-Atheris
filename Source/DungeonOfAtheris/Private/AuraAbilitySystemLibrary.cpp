// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"

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
